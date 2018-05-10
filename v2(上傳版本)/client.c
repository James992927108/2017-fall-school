#include "myftp.h"

char name[10];
char group[10];
char recv_buff[1024];
char send_buff[1024];
char insWrite[1024];
char fileName[100];
unsigned short in_cksum(unsigned short *addr, int len)
{
	int nleft = len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(unsigned char *)(&answer) = *(unsigned char *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

void *pthread_function(void *sock)
{

	int tsock = *(int *)sock;
	memset(recv_buff, 0, sizeof(recv_buff));

	while (1)
	{
		memset(recv_buff, 0, sizeof(recv_buff));

		if (recv(tsock, recv_buff, sizeof(recv_buff), 0) < 0)
		{
			perror("recv error\n");
		}

		printf("server reply: %s\n", recv_buff);

		if (strcmp(recv_buff, "ready to transfer") == 0)
		{

			struct myFtphdr *FRQrequest = (struct myFtphdr *)malloc(MAXLINE);

			//send FRQ_REQUEST to server

			memset(FRQrequest, 0, MAXLINE);
			FRQrequest->mf_opcode = htons(FRQ);
			FRQrequest->mf_cksum = in_cksum((unsigned short *)FRQrequest, MAXLINE);

			if (send(tsock, FRQrequest, MAXLINE, 0) < 0)
			{
				perror("send(FRQ) error --- \n");
			}

			//renew filename

			FILE *fp;
			char clientFN[FNAMELEN];
			strcpy(clientFN, name);
			strcat(clientFN, "'s downloadFile");
			fp = fopen(clientFN, "w");

			//start download

			struct myFtphdr *recvData = (struct myFtphdr *)malloc(MAXLINE);   //data
			struct myFtphdr *sendPacket = (struct myFtphdr *)malloc(MAXLINE); //ack&block

			int block_index = 0;
			int recvFileSize = 0;
			int totalRecvSize = 0;

			while (1)
			{
				memset(recvData, 0, MAXLINE);

				while (recv(tsock, recvData, MAXLINE, 0) < 0)
				{
					perror(" waiting data from server\n");
				}

				//in_cksum == 0 is correct
				if (in_cksum((unsigned short *)recvData, MAXLINE) == 0)
				{

					if (recvData->mf_opcode == htons(DATA))
					{ //DATA

						recvFileSize = fwrite(recvData->mf_data, 1, strlen(recvData->mf_data), fp);
						totalRecvSize += recvFileSize;

						if (recvFileSize < MFMAXDATA)
						{ //last data,size < 512

							// printf("block:%d\n",ntohs(recvData->mf_block));

							memset(sendPacket, 0, MAXLINE);
							block_index = 0;
							sendPacket->mf_block = htons(block_index);
							sendPacket->mf_opcode = htons(ACK);
							sendPacket->mf_cksum = in_cksum((unsigned short *)sendPacket, MAXLINE);

							printf("[file transmission - finish]\n");
							printf("\t\t%d bytes received\n", totalRecvSize);

							send(tsock, sendPacket, MAXLINE, 0);
							fclose(fp);
							break;
						}

						else
						{ // nomal data transmission

							memset(sendPacket, 0, MAXLINE);
							block_index = recvData->mf_block;
							sendPacket->mf_block = htons(block_index);
							sendPacket->mf_opcode = htons(ACK);
							sendPacket->mf_cksum = in_cksum((unsigned short *)sendPacket, MAXLINE);
							send(tsock, sendPacket, MAXLINE, 0);

							//  printf("data block is %d\n",ntohs(block_index));
						}
					}
					else if (recvData->mf_opcode == htons(ERROR))
					{ //ERROR

						if (recvData->mf_block == 0)
						{ //server no recieve FRQ, resend FRQ to server

							memset(FRQrequest, 0, MAXLINE);
							FRQrequest->mf_opcode = htons(FRQ);
							FRQrequest->mf_cksum = in_cksum((unsigned short *)FRQrequest, MAXLINE);
							send(tsock, FRQrequest, MAXLINE, 0);
						}
						else
						{ //resend ack

							memset(sendPacket, 0, MAXLINE);
							sendPacket->mf_opcode = htons(ACK);
							sendPacket->mf_block = htons(block_index);
							sendPacket->mf_cksum = in_cksum((unsigned short *)sendPacket, MAXLINE);
							send(tsock, sendPacket, MAXLINE, 0);
						}
					}
				}
				else
				{ // checksum error !=0

					//need server to resand data
					printf("recieved data checksum error, the block is %d\n", ntohs(recvData->mf_block));

					memset(sendPacket, 0, MAXLINE);
					sendPacket->mf_opcode = htons(ERROR);
					sendPacket->mf_block = htons(recvData->mf_block);
					sendPacket->mf_cksum = in_cksum((unsigned short *)sendPacket, MAXLINE);
					send(tsock, sendPacket, MAXLINE, 0);
				}
			}

			free(recvData);
			free(sendPacket);
		}

		if (strcmp(recv_buff, "ready to write") == 0)
		{
			int block_index = 0;
			int sentFileSize = 0;
			int totalFileSize = 0;

			FILE *file_source;
			file_source = fopen(fileName, "r");

			struct myFtphdr *sendData = (struct myFtphdr *)malloc(MAXLINE);
			struct myFtphdr *recvPacket = (struct myFtphdr *)malloc(MAXLINE);
			memset(recvPacket, 0, MAXLINE);

			while (1)
			{

				while ((recv(tsock, recvPacket, MAXLINE, 0)) < 0)
				{
					printf(" waiting ACK,send data again\n");
				}

				if (in_cksum((unsigned short *)recvPacket, MAXLINE) == 0)
				{ //checksum ==0 ok

					//recv FRQ
					if (recvPacket->mf_opcode == htons(FRQ))
					{
						//send block 1 & data.
						memset(sendData, 0, MAXLINE);
						block_index = 1;
						sendData->mf_block = htons(block_index);
						sendData->mf_opcode = htons(DATA);
						sentFileSize = fread(sendData->mf_data, 1, MFMAXDATA, file_source);
						sendData->mf_cksum = in_cksum((unsigned short *)sendData, MAXLINE);

						send(tsock, sendData, MAXLINE, 0);
						totalFileSize += sentFileSize;
					}

					//recv ACK
					if (recvPacket->mf_opcode == htons(ACK))
					{
						//block index == 0 is finish.
						if (recvPacket->mf_block == 0)
						{
							fclose(file_source);

							printf("\t[ Server Download:<%s> -finish ] \n", fileName);
							printf("\t\t%d bytes sent\n", totalFileSize);

							break;
						}
						else
						{
							//send next data & block
							memset(sendData, 0, MAXLINE);

							block_index++;

							if (block_index == 65536)
							{
								block_index = 1;
							}

							sendData->mf_block = htons(block_index);
							sendData->mf_opcode = htons(DATA);
							sentFileSize = fread(sendData->mf_data, 1, MFMAXDATA, file_source);
							sendData->mf_cksum = in_cksum((unsigned short *)sendData, MAXLINE);
							send(tsock, sendData, MAXLINE, 0);

							totalFileSize += sentFileSize;
						}
					}

					if (recvPacket->mf_opcode == htons(ERROR))
					{
						sendData->mf_block = htons(block_index);
						sendData->mf_opcode = htons(DATA);
						sendData->mf_cksum = 0;
						sendData->mf_cksum = in_cksum((unsigned short *)sendData, MAXLINE);
						send(tsock, sendData, MAXLINE, 0);
					}
				}
				else
				{ //checksum !=0 error
					printf("checksum  error\n");
					memset(sendData, 0, MAXLINE);
					sendData->mf_opcode = htons(ERROR);
					sendData->mf_cksum = in_cksum((unsigned short *)sendData, MAXLINE);
					send(tsock, sendData, MAXLINE, 0);
					break;
				} //end if
				
			}//end while

		}//end if 
	}
	free(sock);
}
void split(char **arr, char *str, const char *del)
{
	char *s = strtok(str, del);
	while (s != NULL)
	{
		*arr++ = s;
		s = strtok(NULL, del);
	}
}
int main(int argc, char *argv[])
{

	/* variables */

	int sockfd;
	struct sockaddr_in servaddr;
	pthread_t pid;

	/* usage information */

	if (argc != 3)
	{
		printf("usage: ./server <port> <IP> \n");
		return 0;
	}

	/* open TCP socket */

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("creat socket error\n");
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr = inet_addr(argv[2]);

	/* connect to server */

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect error\n");
	}

	printf("[Wellcome]\n");
	printf("please enter your name:\n");
	scanf("%s", name);
	send(sockfd, name, strlen(name), 0);
	sleep(0.2);

	printf("please enter your group(aos,cse,other):\n");
	scanf("%s", group);
	send(sockfd, group, strlen(group), 0);
	sleep(0.2);

	printf("Usage command:\n");
	printf("\t<create fileName permission> create new file.\n");
	printf("\t<read fileName> download file.\n");
	printf("\t<write fileName o/a> modify file.\n");
	printf("\t<modify fileName permission> modify file's permission.\n");
	printf("\t<information fileName> show the file's information.\n");

	/* create pthread to recv msg from server */

	pthread_create(&pid, NULL, pthread_function, (void *)&sockfd);

	/* send msg to server */

	while (1)
	{
		memset(send_buff, 0, sizeof(send_buff));
		//printf("enter commad\n");
		gets(send_buff);
		send(sockfd, send_buff, strlen(send_buff), 0);

		char *arr[3];
		const char *del = " ";
		split(arr, send_buff, del); //切割char放入arr
		printf("---%s\n", *(arr));
		printf("---%s\n", *(arr + 1));
		printf("---%s\n", *(arr + 2));
		if (strcmp(*(arr), "write") == 0)
		{
			memset(fileName, 0, sizeof(fileName));
			strcpy(fileName, *(arr + 1));
		}
		sleep(0.2);

		if (strcmp(send_buff, "bye") == 0) //compare bye
		{
			break;
		}
	}

	close(sockfd);
	pthread_cancel(pid);

	return 0;
}
