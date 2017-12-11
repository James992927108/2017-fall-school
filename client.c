#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <arpa/inet.h>

void split(char **arr, char *str, const char *del)
{
	char *s = strtok(str, del);
	while (s != NULL)
	{
		*arr++ = s;
		s = strtok(NULL, del);
	}
}
int main(void)
{
	struct sockaddr_in server;
	int sock;
	pid_t pid;
	int flag = 0;

	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(6789);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	connect(sock, (struct sockaddr *)&server, sizeof(server));

	char recv_userId_from_server[256];
	read(sock, recv_userId_from_server, sizeof(recv_userId_from_server));

	printf("User< %s >\n", recv_userId_from_server);
	int client_state = 0;
	char Member_id[50] = {0};

	pid = fork();
	while (1)
	{ //listen
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			//printf("pid = 0 -- client_state : %d\n", client_state);
			if (client_state == 0)
			{
				printf("1 . Type user name(like : name james)\n");
			}
			else if (client_state == 1)
			{
				printf("2 .which group(like : group AOS-students)\n");
			}
			else if (client_state == 2)
			{
				printf("3 .create file(like : 3 filename rwxrwx---)\n");
				printf("4 .read file\n");
				printf("5 .write file\n");
				printf("6 .modify Permission\n");
			}

			char recv_msg_from_server[5120];
			read(sock, recv_msg_from_server, sizeof(recv_msg_from_server));
			printf("from server :%s \n", recv_msg_from_server);

			if (client_state == 0)
			{
				if (strcmp(recv_msg_from_server, "user_exist") == 0)
				{
					client_state = 2;
					printf("user_exist \n");
				}
				else
				{
					client_state = 1;
				}
			}
			else if (client_state == 1)
			{
				client_state = 2;
			}
			else //client_state = 2;
			{
				char *arr[3];
				const char *del = " ";
				split(arr, recv_msg_from_server, del); //切割char放入arr
				// printf("---%s\n", *(arr));			   //filename
				// printf("---%s\n", *(arr + 1));		   //can_read / can_write
				// printf("---%s\n", *(arr + 2));		   //o / a
				printf("1\n");
				if (strcmp(*(arr + 1), "can_read") == 0)
				{
					FILE *fpInput;
					char ch;
					fpInput = fopen(*(arr), "r");
					while ((ch = fgetc(fpInput)) != EOF)
					{
						putchar(ch);
					}
					fclose(fpInput);
					printf("\n");
				}
				else if (strcmp(*(arr + 1), "can_write") == 0)
				{
					FILE *fpOutput;
					char ch;
					if (strcmp(*(arr + 2), "o") == 0)
					{
						printf("can write o\n");

						fpOutput = fopen(*(arr), "w");
						char buf[5120];

						if (scanf("%27[^\n]%*c", buf) != EOF)
						{
							printf("%s\n", buf);
							fputs(buf, fpOutput);
						}
					}
					else if (strcmp(*(arr + 2), "a") == 0)
					{
						printf("can write a\n");

						fpOutput = fopen(*(arr), "a");
						char buf[5120];

						if (scanf("%27[^\n]%*c", buf) != EOF)
						{
							printf("%s\n", buf);
							fputs(buf, fpOutput);
						}
					}
				
					char mes_to_server[5120] = {0};
					strcpy(mes_to_server, "finish_write ");
					strcat(mes_to_server, *(arr));
					write(sock, mes_to_server, sizeof(mes_to_server));

					printf("%s\n", mes_to_server);
					fclose(fpOutput);
					printf("close\n");
				}
				else
				{
					printf("other message\n");
				}
			}
			if (flag != 0)
			{
				break;
			}
		}
		else
		{
			char buf[5120];

			if (scanf("%27[^\n]%*c", buf) != EOF && strcmp(buf, "quit") != 0)
			{
				write(sock, buf, sizeof(buf));
			}
			else
			{
				flag = 1;
				_exit(EXIT_SUCCESS);
				break;
			}
		}
	}
}
