#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
int a[100] = {0};
int client_num = 0;
typedef struct Permission
{
	char own[3];
	char group[3];
	char other[3];
} Permission;
typedef struct Member
{
	char member_group[50];
	char name[50];
} Member;
//-rw-r----- Ken ASO 87564 Nov 9 2017 homework2.c
typedef struct File
{
	Permission access_right;
	char own_name[50];
	char file_group[50];
	int file_size;
	char create_time[50];
	char file_name[50];
} File;

Member MemberArray[50];
File FileArray[50];
void initial()
{
	int i, j;
	for (i = 0; i < 50; i++)
	{
		for (j = 0; j < 50; j++)
		{
			MemberArray[i].name[j] = '\0';
			MemberArray[i].member_group[j] = '\0';
		}
		//--------------------------------------
		for (j = 0; j < 50; j++)
		{
			FileArray[i].own_name[j] = '\0';
			FileArray[i].file_group[j] = '\0';
			FileArray[i].create_time[j] = '\0';
			FileArray[i].file_name[j] = '\0';
		}
		FileArray[i].file_size = 0;
		FileArray[i].access_right.own[0] = 'r';
		FileArray[i].access_right.own[1] = 'w';
		FileArray[i].access_right.own[2] = 'x';

		FileArray[i].access_right.group[0] = 'r';
		FileArray[i].access_right.group[1] = 'w';
		FileArray[i].access_right.group[2] = 'x';

		FileArray[i].access_right.other[0] = 'r';
		FileArray[i].access_right.other[1] = 'w';
		FileArray[i].access_right.other[2] = 'x';
	}
}
void testData()
{
	strcpy(MemberArray[0].member_group, "AOS-students");
	strcpy(MemberArray[0].name, "james");

	strcpy(MemberArray[1].member_group, "AOS-students");
	strcpy(MemberArray[1].name, "sam");

	strcpy(MemberArray[2].member_group, "CSE-students");
	strcpy(MemberArray[2].name, "antony");

	FileArray[0].access_right.own[0] = 'r';
	FileArray[0].access_right.own[1] = 'w';
	FileArray[0].access_right.own[2] = 'x';
	FileArray[0].access_right.group[0] = 'r';
	FileArray[0].access_right.group[1] = 'w';
	FileArray[0].access_right.group[2] = 'x';
	FileArray[0].access_right.other[0] = 'r';
	FileArray[0].access_right.other[1] = 'w';
	FileArray[0].access_right.other[2] = 'x';

	strcpy(FileArray[0].own_name, MemberArray[0].name);
	strcpy(FileArray[0].file_group, MemberArray[0].member_group);
	FileArray[0].file_size = 0;
	time_t timep;
	time(&timep);
	strcpy(FileArray[0].create_time, asctime(gmtime(&timep)));
	strcpy(FileArray[0].file_name, "test");
}
int check_user_isExist(char *user)
{
	for (int i = 0; i < 50; i++)
	{
		if (strcmp(user, MemberArray[i].name) == 0)
		{
			return 1;
		}
	}
	return 0;
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
	char *buffer;
	int csock = *(int *)sock;
	int readSize;
	long addr = 0;

	int client_id = client_num; //the online id

	a[client_id] = csock;
	char buf[256];
	while (readSize = read(csock, buf, sizeof(buf)))
	{
		printf("from client buf : %s , user id : %d\n", buf, client_id);

		if (strcmp(buf, "quit") == 0)
		{
			client_num--;

			for (int i = client_id; i < 100; i++)
			{
				a[i] = a[i + 1];
				a[99] = 0;
			}

			break;
		}

		if (buf[0] == 'n' && buf[1] == 'a' && buf[2] == 'm' && buf[3] == 'e')
		{
			printf("%s\n", buf);
			printf("%s\n", buf + 5);
			int checkinformation = check_user_isExist(buf + 5);
			if (checkinformation == 1)
			{
				//傳回給使用者，number即client的id
				char mes_to_client[256];
				strcpy(mes_to_client, "user_exist");
				write(a[client_id], mes_to_client, sizeof(mes_to_client));
				printf("%s\n", mes_to_client);
			}
			else
			{
				//建立使用者
				//從蒂9個開始，9前個內建資料庫
				int current_client_id = client_id + 9;
				strcpy(MemberArray[current_client_id].name, buf + 5);
				printf("%d\n", current_client_id);
				printf("%s\n", MemberArray[current_client_id].name);

				char mes_to_client[256];
				strcpy(mes_to_client, "create user successful");
				write(a[client_id], mes_to_client, sizeof(mes_to_client));
				printf("%s\n", mes_to_client);
			}
		}
		else if (buf[0] == 'g' && buf[1] == 'r' && buf[2] == 'o' && buf[3] == 'u' && buf[4] == 'p')
		{
			printf("%s\n", buf);
			printf("%s\n", buf + 6);
			//建立使用者
			//從蒂9個開始，9前個內建資料庫
			int current_client_id = client_id + 9;
			strcpy(MemberArray[current_client_id].member_group, buf + 6);
			printf("%d\n", current_client_id);
			printf("%s\n", MemberArray[current_client_id].member_group);
			char mes_to_client[256];
			strcpy(mes_to_client, "create group successful");
			write(a[client_id], mes_to_client, sizeof(mes_to_client));
			printf("%s\n", mes_to_client);
		}
		else if (buf[0] == '3') //3 .create file
		{
			printf("%s\n", buf);
			printf("%s\n", buf + 2);
			//檢查是否已經有該資料
			const char *del = " ";
			char *arr[50];
			split(arr, buf, del);
			int i = 0;
			while (i < 50)
				printf("123%s\n", *(arr + i++));
			// int checkinformation = check_user_isExist(buf + 2);
			// if (checkinformation == 1)
			// {

			// }
			// else
			// {

			// }
			File creatFile;
			strcpy(creatFile.file_name, buf + 2);

			FILE *file;
			file = fopen(creatFile.file_name, "w");
			//fprintf(file, "%s", "123");
			fclose(file);

			char mes_to_client[256];
			strcpy(mes_to_client, "create file successful");
			write(a[client_id], mes_to_client, sizeof(mes_to_client));
			printf("%s\n", mes_to_client);
		}
		else if (buf[0] == '4') //4 .read file
		{
		}
		else if (buf[0] == '5') //5 .write file
		{
		}
		else if (buf[0] == '6') //6 .modify Permission
		{
		}
	}
	if (readSize == 0)
	{

		printf("Client %d disconnected\n", client_id);
		fflush(stdout);
		client_num--;
		a[client_id] = 0;
		for (int i = client_id; i < 100; i++)
		{
			a[i] = a[i + 1];
			a[99] = 0;
		}
	}
	pthread_exit(0);
}

int main(void)
{
	initial();
	testData();
	struct sockaddr_in server, client;
	int sock, csock, readSize, addressSize, c;
	char buf[256], temp[256];
	char send_client_num[256];
	pthread_t sniffer_thread;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		printf("ERROR opening socket");
		return 1;
	}
	bzero(&server, sizeof(server));

	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(6789);

	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("ERROR on binding");
		return 1;
	}

	listen(sock, 5);

	addressSize = sizeof(client);

	while (1)
	{
		csock = accept(sock, (struct sockaddr *)&server, (socklen_t *)&addressSize);
		//new link
		client_num++;
		printf("online:%d\n", client_num);

		sprintf(send_client_num, "%d", client_num);
		write(csock, send_client_num, sizeof(send_client_num));

		if (csock < 0)
		{
			printf("ERROR on accept");
			return 1;
		}

		if (pthread_create(&sniffer_thread, 0, connection_handler, (void *)&csock) < 0)
		{
			return 1;
		}

		pthread_detach(sniffer_thread);
	}
	return 0;
}
