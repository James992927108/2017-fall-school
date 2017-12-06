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
	int client_state = 0;
	char Member_id[50];
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
				printf("1 . Type user name(like : name-james)\n");
			}
			else if (client_state == 1)
			{
				printf("2 .which group(like : group-AOS-students)\n");
			}
			else if (client_state == 2)
			{
				printf("3 .create file(like : 3-filename.txt)\n");
				printf("4 .read file\n");
				printf("5 .write file\n");
				printf("6 .modify Permission\n");
			}

			char recv_msg_from_server[256];

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
					printf("%s\n", recv_msg_from_server);
				}
			}
			else if (client_state == 1)
			{
				client_state = 2;
				printf("%s\n", recv_msg_from_server);
			}
			else//client_state = 2;
			{
				printf("%s\n", recv_msg_from_server);		
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

			if (scanf("%27[^\n]%*c", buf) != EOF && strcmp(buf, "quit") != 0)
			{
				write(sock, buf, sizeof(buf)); //
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
