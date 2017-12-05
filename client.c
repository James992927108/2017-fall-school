#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <arpa/inet.h>

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
	int check_create_user = 0;
	int client_state = 0;
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
			printf("pid = 0 -- client_state : %d\n", client_state);
			if (client_state == 0)
			{
				printf("1 . Type user name\n");
			}
			else if (client_state == 1)
			{
				printf("2 .which group\n");
			}
			else
			{
				printf("3 .create file\n");
				printf("4 .read file\n");
				printf("5 .write file\n");
				printf("6 .modify Permission\n");
			}

			char recv_from_server[256];

			read(sock, recv_from_server, sizeof(recv_from_server));

			//recvbox[readFromsock] = 0;
			printf("from server :%s \n", recv_from_server);

			if (strcmp(recv_from_server, "exist") == 0)
			{
				client_state = 2;
				
				printf("user have created before \n");
			}
			else
			{
				client_state = 1;

				printf("creating user \n");
			}
			if (flag != 0)
			{
				break;
			}
		}
		else
		{
			char buf[256];
			printf("pid > 0 -- client_state :  %d\n", client_state);

			if (client_state == 0)
			{
				if (scanf("%27[^\n]%*c", buf) != EOF && strcmp(buf, "quit") != 0)
				{
					char name_tage[256];
					strcpy(name_tage, "name-");
					strcat(name_tage, buf);
					write(sock, name_tage, sizeof(name_tage)); //要傳一個tag_name，告訴server為name
				}
				else
				{
					flag = 1;
					_exit(EXIT_SUCCESS);
					break;
				}
			}
			else if (client_state == 1)
			{

				if (scanf("%27[^\n]%*c", buf) != EOF && strcmp(buf, "quit") != 0)
				{
					char name_tage[256];
					strcpy(name_tage, "group-");
					strcat(name_tage, buf);
					write(sock, name_tage, sizeof(name_tage)); //要傳一個tag_name，告訴server為name
				}
				else
				{
					flag = 1;
					_exit(EXIT_SUCCESS);
					break;
				}
			}
			else
			{
			}
		}
	}
}
