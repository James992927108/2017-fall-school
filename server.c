#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
int a[100] = {0};
int client_num = 0;
// typedef struct Permission
// {
// 	char own[3];
// 	char group[3];
// 	char other[3];
// } Permission;
typedef struct Member
{
	char member_group[50];
	char name[50];
} Member;
//-rw-r----- Ken ASO 87564 Nov 9 2017 homework2.c
typedef struct File
{
	char access_right[9];
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
		FileArray[i].access_right[0] = 'r';
		FileArray[i].access_right[1] = 'w';
		FileArray[i].access_right[2] = 'x';

		FileArray[i].access_right[3] = 'r';
		FileArray[i].access_right[4] = 'w';
		FileArray[i].access_right[5] = 'x';

		FileArray[i].access_right[6] = 'r';
		FileArray[i].access_right[7] = 'w';
		FileArray[i].access_right[8] = 'x';
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

	FileArray[0].access_right[0] = 'r';
	FileArray[0].access_right[1] = 'w';
	FileArray[0].access_right[2] = 'x';
	FileArray[0].access_right[3] = 'r';
	FileArray[0].access_right[4] = 'w';
	FileArray[0].access_right[5] = 'x';
	FileArray[0].access_right[6] = 'r';
	FileArray[0].access_right[7] = 'w';
	FileArray[0].access_right[8] = 'x';

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
int check_file_isExist(char *filename)
{
	for (int i = 0; i < 50; i++)
	{
		if (strcmp(filename, FileArray[i].file_name) == 0)
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
//-rw-r----- Ken ASO 87564 Nov 9 2017 homework2.c
void sendFormat(File file, char *mes_to_client)
{
	char file_size_temp[128] = {0};
	char block[1] = {" "};
	//-rw-r----- Ken ASO 87564 Nov 9 2017 homework2.c
	//rwx---rwxjack jack nctu 0 0Thu Dec  7 13:21:39 2017 0filename.txt
	printf("%s\n",file.access_right);

	strcat(mes_to_client, file.access_right);
	strcat(mes_to_client, block);
	strcat(mes_to_client, file.own_name);
	strcat(mes_to_client, block);
	strcat(mes_to_client, file.file_group);
	strcat(mes_to_client, block);
	sprintf(file_size_temp, "%d", file.file_size);
	strcat(mes_to_client, file_size_temp);
	strcat(mes_to_client, block);
	strcat(mes_to_client, file.create_time);
	strcat(mes_to_client, block);
	strcat(mes_to_client, file.file_name);
	printf("sendFormat finish\n");
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
		char *arr[3];
		const char *del = " ";
		split(arr, buf, del); //切割char放入arr
		printf("---%s\n", *(arr));
		printf("---%s\n", *(arr + 1));
		// printf("---%s\n", *(arr + 2));
		if (strcmp(*(arr), "quit") == 0)
		{
			client_num--;

			for (int i = client_id; i < 100; i++)
			{
				a[i] = a[i + 1];
				a[99] = 0;
			}

			break;
		}

		if (strcmp(*(arr), "name") == 0)
		{
			int checkinformation = check_user_isExist(*(arr + 1));
			if (checkinformation == 1)
			{
				//傳回給使用者
				char mes_to_client[256] = {0};
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
				printf("%s\n", MemberArray[current_client_id].name);

				char mes_to_client[256] = {0};
				strcpy(mes_to_client, "create user successful");
				write(a[client_id], mes_to_client, sizeof(mes_to_client));
				printf("%s\n", mes_to_client);
			}
		}
		else if (strcmp(*(arr), "group") == 0)
		{
			//建立使用者
			//從蒂9個開始，9前個內建資料庫
			int current_client_id = client_id + 9;
			strcpy(MemberArray[current_client_id].member_group, buf + 6);
			printf("%s\n", MemberArray[current_client_id].member_group);

			char mes_to_client[256] = {0};
			strcpy(mes_to_client, "create group successful");
			write(a[client_id], mes_to_client, sizeof(mes_to_client));
			printf("%s\n", mes_to_client);
		}
		else if (strcmp(*(arr), "3") == 0) //3 .create file
		{
			//有可能沒輸入名子就進入3，需要做防呆
			//檢查是否已經有該資料
			int checkinformation = check_file_isExist(*(arr + 1));
			if (checkinformation == 1) //存在
			{
				//傳回給使用者
				char mes_to_client[256];
				strcpy(mes_to_client, "file_exist");
				write(a[client_id], mes_to_client, sizeof(mes_to_client));
				printf("%s\n", mes_to_client);
			}
			else
			{

				//目前一個使用者先暫定只能建立一個file，尚未做防呆
				char filename[50];
				strcpy(filename, *(arr + 1));
				char access_right[9];
				strcpy(access_right, *(arr + 2));

				time_t timep;
				time(&timep);

				FILE *file;
				file = fopen(filename, "w"); //建立檔案
				fclose(file);
				int current_client_id = client_id + 9;

				FileArray[current_client_id].access_right[0] = access_right[0];
				FileArray[current_client_id].access_right[1] = access_right[1];
				FileArray[current_client_id].access_right[2] = access_right[2];

				FileArray[current_client_id].access_right[3] = access_right[3];
				FileArray[current_client_id].access_right[4] = access_right[4];
				FileArray[current_client_id].access_right[5] = access_right[5];

				FileArray[current_client_id].access_right[6] = access_right[6];
				FileArray[current_client_id].access_right[7] = access_right[7];
				FileArray[current_client_id].access_right[8] = access_right[8];

				strcpy(FileArray[current_client_id].own_name, MemberArray[current_client_id].name);
				strcpy(FileArray[current_client_id].file_group, MemberArray[current_client_id].member_group);

				strcpy(FileArray[current_client_id].create_time, asctime(gmtime(&timep)));
				strcpy(FileArray[current_client_id].file_name, filename);
				FileArray[current_client_id].file_size = 0;

				char mes_to_client[512] = {0};
				sendFormat(FileArray[current_client_id], &mes_to_client);
				write(a[client_id], mes_to_client, sizeof(mes_to_client));
				printf("****%s\n", mes_to_client);
			}
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
