#include "myftp.h"

#define count 10
typedef struct File
{
	char file_name[49];
	char own_name[49];
	char file_group[49];
	int file_size;
	char year[49];
	char month[49];
	char data[49];
	char access_right[10];
	int is_write; //用於判斷該檔案有無被開啟修改
	char history_access_right[10][10];
} File;
typedef struct Member
{
	char name[49];
	char member_group[49];
} Member;
File FileArray[count];
char clientInfo[count][2][count]; // [clientName] [group] [char len]
char fileInfo[count][13][count];  // fileInfo

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
void split(char **arr, char *str, const char *del)
{
	char *s = strtok(str, del);
	while (s != NULL)
	{
		*arr++ = s;
		s = strtok(NULL, del);
	}
}
void *connection_handler(void *sock)
{

	char send_buff[1024];
	char recv_buff[1024];
	char name[10];
	char group[10];
	int i, j, k, x, y, z;
	int tsock = *(int *)sock;
	char temp[5];
	char fileName[20];
	char tempPermission[10];
	char new_tempPermission[10];
	char new_fileSize[20];
	char year[5];
	char month[5];
	char date[5];
	char own_r[2];
	char own_w[2];
	char group_r[2];
	char group_w[2];
	char othet_r[2];
	char other_w[2];

	struct tm *p;
	struct stat filestate;

	/* recv client name into client table */

	memset(name, 0, sizeof(name));
	if (recv(tsock, name, sizeof(name), 0) < 0)
	{
		perror("recv Client name error\n");
	}

	for (i = 0, j = 0, k = 0; i < count; i++)
	{
		if (strcmp(&clientInfo[i][j][k], "\0") == 0)
		{
			strcpy(&clientInfo[i][j][k], name);
			break;
		}
	}

	sleep(0.2);

	/* put the client group into client table */

	memset(group, 0, sizeof(group));
	if (recv(tsock, group, sizeof(group), 0) < 0)
	{
		perror("recv Client group error\n");
	}

	for (i = 0, j = 1, k = 0; i < count; i++)
	{
		if (strcmp(&clientInfo[i][j][k], "\0") == 0)
		{
			strcpy(&clientInfo[i][j][k], group);
			break;
		}
	}

	sleep(0.2);

	printf("<%s/%s> login\n", name, group);

	while (1)
	{
		memset(recv_buff, 0, sizeof(recv_buff));
		if (recv(tsock, recv_buff, sizeof(recv_buff), 0) < 0)
		{
			perror("recv Client msg error\n");
		}
		char *arr[3];
		const char *del = " ";
		split(arr, recv_buff, del);
		//取得檔名，位於arr 第二格
		memset(fileName, 0, sizeof(fileName));
		strcpy(fileName, *(arr + 1));

		//printf("buff:%s\n",recv_buff);

		/*--------------------- new command----------------------*/

		if (recv_buff[0] == 'c' && recv_buff[1] == 'r' && recv_buff[2] == 'e' && recv_buff[3] == 'a' && recv_buff[4] == 't')
		{

			char fileInfor[100];
			memset(fileInfor, 0, sizeof(fileInfor));

			strcpy(tempPermission, *(arr + 2));

			for (i = 0, j = 0, k = 0; i < count; i++)
			{
				if (strcmp(&fileInfo[i][j][k], fileName) == 0)
				{ // filename exist
					strcpy(send_buff, "The file has existed, can't use this filename.");
					send(tsock, send_buff, strlen(send_buff), 0);
					memset(send_buff, 0, sizeof(send_buff)); //清空
				}
				else
				{ // filename not exist, open new file
					FILE *fp;
					fp = fopen(fileName, "w");

					int n = 0;
					sprintf(new_fileSize, "%d", n); // fizesize int->char

					time_t timep; // get time
					time(&timep);
					p = gmtime(&timep);
					//printf("%d %d %d\n",(1+p->tm_mon),(1+p->tm_mday),(1900+p->tm_year));
					sprintf(year, "%d", (1900 + p->tm_year));
					sprintf(month, "%d", (1 + p->tm_mon));
					sprintf(date, "%d", (1 + p->tm_mday));

					strncpy(new_tempPermission, tempPermission, 6);

					// put permission in fileInfo.table
					own_r[x] = new_tempPermission[x];
					own_w[x] = new_tempPermission[x + 1];
					group_r[x] = new_tempPermission[x + 2];
					group_w[x] = new_tempPermission[x + 3];
					othet_r[x] = new_tempPermission[x + 4];
					other_w[x] = new_tempPermission[x + 5];

					for (x = 0, y = 0, z = 0; x < count; x++)
					{ // put fileInfo in table
						if (strcmp(&fileInfo[x][y][z], "\0") == 0)
						{
							strcpy(&fileInfo[x][y][z], fileName);		  // fileName
							strcpy(&fileInfo[x][y + 1][z], name);		  // Creater
							strcpy(&fileInfo[x][y + 2][z], group);		  // group
							strcpy(&fileInfo[x][y + 3][z], new_fileSize); // fileSize
							strcpy(&fileInfo[x][y + 4][z], year);		  // year
							strcpy(&fileInfo[x][y + 5][z], month);		  // month
							strcpy(&fileInfo[x][y + 6][z], date);		  // date
							strcpy(&fileInfo[x][y + 7][z], own_r);		  // own_r
							strcpy(&fileInfo[x][y + 8][z], own_w);		  // own_w
							strcpy(&fileInfo[x][y + 9][z], group_r);	  // group_r
							strcpy(&fileInfo[x][y + 10][z], group_w);	 // group_w
							strcpy(&fileInfo[x][y + 11][z], othet_r);	 // othet_r
							strcpy(&fileInfo[x][y + 12][z], other_w);	 // other_w

							break;
						}
					}

					break;
				} //end else
			}	 // end for

			strcpy(send_buff, "Create new file success.");
			send(tsock, send_buff, strlen(send_buff), 0);
			memset(send_buff, 0, sizeof(send_buff));

			for (i = 0, j = 0, k = 0; i < count; i++)
			{

				if (strcmp(&fileInfo[i][j][k], fileName) == 0)
				{
					printf("<new file> FileName:%s Creater:%s Group:%s FileSize:%s Date:%s %s %s Permission: %s%s%s%s%s%s\n", &fileInfo[i][j][k], &fileInfo[i][j + 1][k], &fileInfo[i][j + 2][k], &fileInfo[i][j + 3][k], &fileInfo[i][j + 4][k], &fileInfo[i][j + 5][k], &fileInfo[i][j + 6][k], &fileInfo[i][j + 7][k], &fileInfo[i][j + 8][k], &fileInfo[i][j + 9][k], &fileInfo[i][j + 10][k], &fileInfo[i][j + 11][k], &fileInfo[i][j + 12][k]);
				}
			}
		}

		/*--------------------- read command----------------------*/

		else if (recv_buff[0] == 'r' && recv_buff[1] == 'e' && recv_buff[2] == 'a' && recv_buff[3] == 'd')
		{

			int step = 0;

			/* check if file exist. */

			if (lstat(fileName, &filestate) < 0)
			{ // recorrect
				printf("unknow file : %s\n", fileName);
				strcpy(send_buff, "Not exist file");
				send(tsock, send_buff, strlen(send_buff), 0);
				memset(send_buff, 0, sizeof(send_buff));
				return 0;
			}

			/* check client permission */

			for (i = 0, j = 0, k = 0; i < count; i++)
			{
				if (strcmp(&fileInfo[i][j][k], fileName) == 0) //取得檔案得位置後
				{
					if (strcmp(&fileInfo[i][j + 1][k], name) == 0) //比對creater
					{
						strcpy(send_buff, "ready to transfer");
						send(tsock, send_buff, strlen(send_buff), 0);
						sleep(0.2);
						memset(send_buff, 0, sizeof(send_buff));
						step++;
					}
					else if (strcmp(&fileInfo[i][j + 2][k], group) == 0) //比對group
					{
						if (strcmp(&fileInfo[i][j + 9][k], "r") == 0)
						{
							strcpy(send_buff, "ready to transfer");
							send(tsock, send_buff, strlen(send_buff), 0);
							sleep(0.2);
							memset(send_buff, 0, sizeof(send_buff));
							step++;
						}
						else
						{
							strcpy(send_buff, "You don't have permission to read.");
							send(tsock, send_buff, strlen(send_buff), 0);
							memset(send_buff, 0, sizeof(send_buff));
							break;
						}
					}

					else
					{
						if (strcmp(&fileInfo[i][j + 11][k], "r") == 0)
						{
							strcpy(send_buff, "ready to transfer");
							send(tsock, send_buff, strlen(send_buff), 0);
							sleep(0.2);
							memset(send_buff, 0, sizeof(send_buff));
							step++;
						}
						else
						{
							strcpy(send_buff, "You don't have permission to read.");
							send(tsock, send_buff, strlen(send_buff), 0);
							memset(send_buff, 0, sizeof(send_buff));
							break;
						}
					}
				}
			}

			if (step == 1)
			{

				char tmpown_r[2];
				char tmpAOS_w[2];
				char tmpCSE_r[2];
				char tmpgroup_w[2];
				char tmpothet_r[2];
				char tmpother_w[2];

				memset(tmpown_r, 0, sizeof(tmpown_r));
				memset(tmpAOS_w, 0, sizeof(tmpAOS_w));
				memset(tmpCSE_r, 0, sizeof(tmpCSE_r));
				memset(tmpgroup_w, 0, sizeof(tmpCSE_r));
				memset(tmpothet_r, 0, sizeof(tmpothet_r));
				memset(tmpothet_r, 0, sizeof(tmpothet_r));

				for (i = 0, j = 0, k = 0; i < count; i++)
				{
					if (strcmp(&fileInfo[i][j][k], fileName) == 0)
					{
						strcpy(tmpown_r, &fileInfo[i][j + 7][k]); // copy permission to tmp
						strcpy(tmpAOS_w, &fileInfo[i][j + 8][k]);
						strcpy(tmpCSE_r, &fileInfo[i][j + 9][k]);
						strcpy(tmpgroup_w, &fileInfo[i][j + 10][k]);
						strcpy(tmpothet_r, &fileInfo[i][j + 11][k]);
						strcpy(tmpother_w, &fileInfo[i][j + 12][k]);
						break;
					}
				}

				//  start transfer

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

								printf("\t[From:<%s> Download:<%s> -finish ] \n", name, fileName);
								printf("\t\t%d bytes sent\n", totalFileSize);

								for (i = 0, j = 0, k = 0; i < count; i++)
								{
									if (strcmp(&fileInfo[i][j][k], fileName) == 0)
									{

										strcpy(&fileInfo[i][j + 7][k], tmpown_r); // recover permission
										strcpy(&fileInfo[i][j + 8][k], tmpAOS_w);
										strcpy(&fileInfo[i][j + 9][k], tmpCSE_r);
										strcpy(&fileInfo[i][j + 10][k], tmpgroup_w);
										strcpy(&fileInfo[i][j + 11][k], tmpothet_r);
										strcpy(&fileInfo[i][j + 12][k], tmpother_w);
									}
								}

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
				}

				free(recvPacket);
				free(sendData);

				strcpy(send_buff, "transfer file success.");
				send(tsock, send_buff, strlen(send_buff), 0);
				memset(send_buff, 0, sizeof(send_buff));
			} // end step=1
		}

		/*--------------------- write command----------------------*/

		else if (recv_buff[0] == 'w' && recv_buff[1] == 'r' && recv_buff[2] == 'i' && recv_buff[3] == 't' && recv_buff[4] == 'e')
		{
			char instruction[2];
			strcpy(instruction, *(arr + 2));

			/* check if file exist. */

			if (lstat(fileName, &filestate) < 0)
			{ // recorrect
				printf("unknow file : %s\n", fileName);
				strcpy(send_buff, "Not exist file");
				send(tsock, send_buff, strlen(send_buff), 0);
				memset(send_buff, 0, sizeof(send_buff));
				return 0;
			}

			/* check client permission */

			int step = 0;

			//檔案存在則做下列權限判斷
			//

			for (i = 0, j = 0, k = 0; i < count; i++)
			{
				if (strcmp(&fileInfo[i][j][k], fileName) == 0) //取得檔案得位置後
				{
					//先判斷該使用者是否為創建者
					//否則以group判斷
					//最後other判斷
					if (strcmp(&fileInfo[i][j + 1][k], name) == 0) //比對creater
					{
						strcpy(send_buff, "ready to write");
						send(tsock, send_buff, strlen(send_buff), 0);
						sleep(0.2);
						memset(send_buff, 0, sizeof(send_buff));
						step++;
					}
					else if (strcmp(&fileInfo[i][j + 2][k], group) == 0) //比對group_w
					{
						if (strcmp(&fileInfo[i][j + 10][k], "w") == 0)
						{
							strcpy(send_buff, "ready to write");
							send(tsock, send_buff, strlen(send_buff), 0);
							sleep(0.2);
							memset(send_buff, 0, sizeof(send_buff));
							step++;
						}
						else
						{
							strcpy(send_buff, "You don't have permission to write.");
							send(tsock, send_buff, strlen(send_buff), 0);
							sleep(0.2);
							memset(send_buff, 0, sizeof(send_buff));
							break;
						}
					}
					else
					{
						if (strcmp(&fileInfo[i][j + 12][k], "w") == 0) //比對other_w
						{
							strcpy(send_buff, "ready to write");
							send(tsock, send_buff, strlen(send_buff), 0);
							sleep(0.2);
							memset(send_buff, 0, sizeof(send_buff));
							step++;
						}
						else
						{
							strcpy(send_buff, "You don't have permission to write.");
							send(tsock, send_buff, strlen(send_buff), 0);
							sleep(0.2);
							memset(send_buff, 0, sizeof(send_buff));
							break;
						}
					}
				}
			}

			/* start to write */

			if (step == 1)
			{

				/* when someone write, close file's permission */
				char tmpown_r[2];
				char tmpAOS_w[2];
				char tmpCSE_r[2];
				char tmpgroup_w[2];
				char tmpothet_r[2];
				char tmpother_w[2];

				memset(tmpown_r, 0, sizeof(tmpown_r));
				memset(tmpAOS_w, 0, sizeof(tmpAOS_w));
				memset(tmpCSE_r, 0, sizeof(tmpCSE_r));
				memset(tmpgroup_w, 0, sizeof(tmpCSE_r));
				memset(tmpothet_r, 0, sizeof(tmpothet_r));
				memset(tmpothet_r, 0, sizeof(tmpothet_r));

				for (i = 0, j = 0, k = 0; i < count; i++)
				{
					if (strcmp(&fileInfo[i][j][k], fileName) == 0)
					{
						strcpy(tmpown_r, &fileInfo[i][j + 7][k]); // copy permission to tmp
						strcpy(tmpAOS_w, &fileInfo[i][j + 8][k]);
						strcpy(tmpCSE_r, &fileInfo[i][j + 9][k]);
						strcpy(tmpgroup_w, &fileInfo[i][j + 10][k]);
						strcpy(tmpothet_r, &fileInfo[i][j + 11][k]);
						strcpy(tmpother_w, &fileInfo[i][j + 12][k]);

						strcpy(&fileInfo[i][j + 7][k], "-"); //close permission
						strcpy(&fileInfo[i][j + 8][k], "-");
						strcpy(&fileInfo[i][j + 9][k], "-");
						strcpy(&fileInfo[i][j + 10][k], "-");
						strcpy(&fileInfo[i][j + 11][k], "-");
						strcpy(&fileInfo[i][j + 12][k], "-");
						break;
					}
				}

				/* start write */
				int FileSize = 0;
				int totalFileSize = 0;
				char new[20];
				struct clientWrite wr;
				FILE *fp;

				if (strcmp(instruction, "o") == 0)
				{ // write o
					fp = fopen(fileName, "w");
				}
				else
				{ // write a
					fp = fopen(fileName, "a");
				}
				/* start teansfrom */
				struct myFtphdr *FRQrequest = (struct myFtphdr *)malloc(MAXLINE);
				//send frq to client
				memset(FRQrequest, 0, MAXLINE);
				FRQrequest->mf_opcode = htons(FRQ);
				FRQrequest->mf_cksum = in_cksum((unsigned short *)FRQrequest, MAXLINE);

				if (send(tsock, FRQrequest, MAXLINE, 0) < 0)
				{
					perror("send(FRQ) error --- \n");
				}																  //end else
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
						perror(" waiting data from client\n");
					}

					//------------------------------------------------------------------
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
					//-------------------------------------------------------
					// FileSize = fwrite(wr.insW, 1, strlen(wr.insW), fp);
					// totalFileSize += FileSize;
				} //end while

				free(recvData);
				free(sendPacket);

				for (i = 0, j = 0, k = 0; i < count; i++)
				{
					if (strcmp(&fileInfo[i][j][k], fileName) == 0)
					{
						sprintf(new, "%d", totalFileSize); // renew filesize
						strcpy(&fileInfo[i][j + 3][k], new);

						strcpy(&fileInfo[i][j + 7][k], tmpown_r); // recover permission
						strcpy(&fileInfo[i][j + 8][k], tmpAOS_w);
						strcpy(&fileInfo[i][j + 9][k], tmpCSE_r);
						strcpy(&fileInfo[i][j + 10][k], tmpgroup_w);
						strcpy(&fileInfo[i][j + 11][k], tmpothet_r);
						strcpy(&fileInfo[i][j + 12][k], tmpother_w);
					}
				}

				strcpy(send_buff, "write success");
				send(tsock, send_buff, strlen(send_buff), 0);
				memset(send_buff, 0, sizeof(send_buff));
				sleep(0.2);
				printf("<%s> modify(o) the <%s> success\n", name, fileName);

			} //end step 1
		}
		/*--------------------- change command----------------------*/

		else if (recv_buff[0] == 'm' && recv_buff[1] == 'o' && recv_buff[2] == 'd' && recv_buff[3] == 'i' && recv_buff[4] == 'f' && recv_buff[5] == 'y')
		{
			memset(tempPermission, 0, sizeof(tempPermission));
			memset(new_tempPermission, 0, sizeof(new_tempPermission));
			memset(own_r, 0, sizeof(own_r));
			memset(own_w, 0, sizeof(own_w));
			memset(group_r, 0, sizeof(group_r));
			memset(group_w, 0, sizeof(group_w));
			memset(othet_r, 0, sizeof(othet_r));
			memset(other_w, 0, sizeof(other_w));

			strcpy(tempPermission, *(arr + 2));

			strncpy(new_tempPermission, tempPermission, 6);

			for (x = 0; x < 1; x++)
			{ // put permission in fileInfo.table
				own_r[x] = new_tempPermission[x];
				own_w[x] = new_tempPermission[x + 1];
				group_r[x] = new_tempPermission[x + 2];
				group_w[x] = new_tempPermission[x + 3];
				othet_r[x] = new_tempPermission[x + 4];
				other_w[x] = new_tempPermission[x + 5];
			}

			/* compare creater */

			for (i = 0, j = 0, k = 0; i < count; i++)
			{
				if (strcmp(&fileInfo[i][j][k], fileName) == 0)
				{ // first compare filename
					if (strcmp(&fileInfo[i][j + 1][k], name) == 0)
					{											  // then compare creater
						strcpy(&fileInfo[i][j + 7][k], own_r);	// own_r
						strcpy(&fileInfo[i][j + 8][k], own_w);	// own_w
						strcpy(&fileInfo[i][j + 9][k], group_r);  // group_r
						strcpy(&fileInfo[i][j + 10][k], group_w); // group_w
						strcpy(&fileInfo[i][j + 11][k], othet_r); // othet_r
						strcpy(&fileInfo[i][j + 12][k], other_w); // other_w

						strcpy(send_buff, "change file permission success.");
						send(tsock, send_buff, strlen(send_buff), 0);
						printf("<%s> change <%s> permission:%s%s%s%s%s%s success\n", name, fileName, own_r, own_w, group_r, group_w, othet_r, other_w);
						sleep(0.2);
						memset(send_buff, 0, sizeof(send_buff));

						break;
					}
					else
					{
						strcpy(send_buff, "you have no permission to change file's permission.");
						send(tsock, send_buff, strlen(send_buff), 0);
						sleep(0.2);
						memset(send_buff, 0, sizeof(send_buff));
						break;
					}
				}
			}

			/*for(i=0,j=0,k=0;i<count;i++){
		
				if(strcmp(&fileInfo[i][j][k],"\0") != 0){
					printf("fileName:%s\n",&fileInfo[i][j][k]);
					printf("creater:%s\n",&fileInfo[i][j+1][k]);
					printf("group:%s\n",&fileInfo[i][j+2][k]);
					printf("fileSize:%s\n",&fileInfo[i][j+3][k]);
					printf("year:%s\n",&fileInfo[i][j+4][k]);
					printf("month:%s\n",&fileInfo[i][j+5][k]);
					printf("date:%s\n",&fileInfo[i][j+6][k]);
					printf("own_r:%s\n",&fileInfo[i][j+7][k]);
					printf("own_w:%s\n",&fileInfo[i][j+8][k]);
					printf("group_r:%s\n",&fileInfo[i][j+9][k]);
					printf("group_w:%s\n",&fileInfo[i][j+10][k]);
					printf("othet_r:%s\n",&fileInfo[i][j+11][k]);
					printf("other_w:%s\n",&fileInfo[i][j+12][k]);
				}			
			}*/
		}

		/*--------------------- Info command----------------------*/

		else if (recv_buff[0] == 'i' && recv_buff[1] == 'n' && recv_buff[2] == 'f' && recv_buff[3] == 'o' && recv_buff[4] == 'r' && recv_buff[5] == 'm' && recv_buff[6] == 'a' && recv_buff[7] == 't' && recv_buff[8] == 'i' && recv_buff[9] == 'o' && recv_buff[10] == 'n')
		{

			char fileInfor[100];
			memset(fileInfor, 0, sizeof(fileInfor));
			memset(fileName, 0, sizeof(fileName));

			for (i = 0, j = 0, k = 0; i < count; i++)
			{

				if (strcmp(&fileInfo[i][j][k], fileName) == 0)
				{
					strcat(fileInfor, "File:");
					strcat(fileInfor, &fileInfo[i][j][k]);
					strcat(fileInfor, " ");
					strcat(fileInfor, "Creater:");
					strcat(fileInfor, &fileInfo[i][j + 1][k]);
					strcat(fileInfor, " ");
					strcat(fileInfor, "Group:");
					strcat(fileInfor, &fileInfo[i][j + 2][k]);
					strcat(fileInfor, " ");
					strcat(fileInfor, "Filesize:");
					strcat(fileInfor, &fileInfo[i][j + 3][k]);
					strcat(fileInfor, " ");
					strcat(fileInfor, "Date:");
					strcat(fileInfor, &fileInfo[i][j + 4][k]);
					strcat(fileInfor, " ");
					strcat(fileInfor, &fileInfo[i][j + 5][k]);
					strcat(fileInfor, " ");
					strcat(fileInfor, &fileInfo[i][j + 6][k]);
					strcat(fileInfor, " ");
					strcat(fileInfor, "Permission:");
					strcat(fileInfor, &fileInfo[i][j + 7][k]);
					strcat(fileInfor, &fileInfo[i][j + 8][k]);
					strcat(fileInfor, &fileInfo[i][j + 9][k]);
					strcat(fileInfor, &fileInfo[i][j + 10][k]);
					strcat(fileInfor, &fileInfo[i][j + 11][k]);
					strcat(fileInfor, &fileInfo[i][j + 12][k]);

					break;
				}
			}

			send(tsock, fileInfor, strlen(fileInfor), 0);
			sleep(0.2);
			memset(fileInfor, 0, sizeof(fileInfor));

			/*for(i=0,j=0,k=0;i<count;i++){
		
				if(strcmp(&fileInfo[i][j][k],fileName) == 0){
					printf("fileName:%s\n",&fileInfo[i][j][k]);
					printf("creater:%s\n",&fileInfo[i][j+1][k]);
					printf("group:%s\n",&fileInfo[i][j+2][k]);
					printf("fileSize:%s\n",&fileInfo[i][j+3][k]);
					printf("year:%s\n",&fileInfo[i][j+4][k]);
					printf("month:%s\n",&fileInfo[i][j+5][k]);
					printf("date:%s\n",&fileInfo[i][j+6][k]);
					printf("own_r:%s\n",&fileInfo[i][j+7][k]);
					printf("own_w:%s\n",&fileInfo[i][j+8][k]);
					printf("group_r:%s\n",&fileInfo[i][j+9][k]);
					printf("group_w:%s\n",&fileInfo[i][j+10][k]);
					printf("othet_r:%s\n",&fileInfo[i][j+11][k]);
					printf("other_w:%s\n",&fileInfo[i][j+12][k]);
				}	
		
			}*/
		}

		else if (recv_buff[0] == 'b' && recv_buff[1] == 'y' && recv_buff[2] == 'e' && recv_buff[3] == '\0')
		{
			break;
		}

		else
		{
			strcpy(send_buff, recv_buff);
			send(tsock, send_buff, strlen(send_buff), 0);
			memset(send_buff, 0, sizeof(send_buff));
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	/* variables */

	int i = 0, j = 0;
	int sockfd, client_fd;
	struct sockaddr_in servaddr, clientaddr;
	socklen_t len = sizeof(struct sockaddr);
	pthread_t id;

	/* usage information */

	if (argc != 2)
	{
		printf("usage: ./server <port> \n");
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
	servaddr.sin_addr.s_addr = INADDR_ANY;

	/* bind socket */

	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind error\n");
	}

	/* listen client */

	if (listen(sockfd, 8) < 0)
	{
		perror("listen error\n");
	}

	printf("Server start!!\n");

	/* accept client */

	while (1)
	{

		if ((client_fd = accept(sockfd, (struct sockaddr *)&clientaddr, &len)) < 0)
		{
			perror("accept error\n");
		}

		/* create thread */

		pthread_create(&id, NULL, connection_handler, (void *)&client_fd);
	}

	return 0;
}
