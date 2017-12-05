#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
int a[100] = {0};
int working = 0;
int Member_id = 0;
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
	strcpy(MemberArray[Member_id].member_group, "AOS-students");
	strcpy(MemberArray[Member_id].name, "james");
	Member_id++;
	strcpy(MemberArray[Member_id].member_group, "AOS-students");
	strcpy(MemberArray[Member_id].name, "sam");
	Member_id++;
	strcpy(MemberArray[Member_id].member_group, "CSE-students");
	strcpy(MemberArray[Member_id].name, "antony");
	Member_id++;

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
void *connection_handler(void *sock)
{
	char *buffer;
	int csock = *(int *)sock;
	int readSize;
	long addr = 0;
	char buf[256], temp[256];
	
	int i, j;

	int number = working; //the online id

	a[number] = csock;

	while (readSize = read(csock, buf, sizeof(buf)))
	{

		buf[readSize] = 0;
		printf("user : %s , user id : %d\n", buf, number);
		
	
		if (strcmp(buf, "quit") == 0)
		{
			working--;

			for (i = number; i < 100; i++)
			{
				a[i] = a[i + 1];
				a[99] = 0;
			}

			break;
		}
		if (buf[0] == 'n' && buf[1] == 'a' && buf[2] == 'm' && buf[3] == 'e')
		{
			printf("%s\n", buf + 5);
			int checkinformation = check_user_isExist(buf + 5);
			if (checkinformation == 1)
			{
				//傳回給使用者，number使用者的id
				char mes_to_client[256];
				strcpy(mes_to_client, "exist");
				write(a[number], mes_to_client, sizeof(mes_to_client));
				printf("%s\n", mes_to_client);
			}
			else
			{
				//建立使用者
				// strcpy(MemberArray[Member_id].member_group, "AOS-students");
				strcpy(MemberArray[Member_id].name, buf);
				printf("%d\n", Member_id);
				printf("%s\n", MemberArray[Member_id++].name);

				char mes_to_client[256];
				strcpy(mes_to_client, "create successful");
				write(a[number], mes_to_client, sizeof(mes_to_client));
				printf("create successful\n");
			}
		}
		if (buf[0] == 'g' && buf[1] == 'r' && buf[2] == 'o' && buf[3] == 'u' && buf[4] == 'p')
		{
			printf("%s\n", buf);
			printf("%s\n", buf + 6);
		}

	}
	if (readSize == 0)
	{

		printf("Client %d disconnected\n", number);
		fflush(stdout);
		working--;
		a[number] = 0;
		for (i = number; i < 100; i++)
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
	char cworking[256];
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
		working++;
		printf("online:%d\n", working);

		sprintf(cworking, "%d", working);
		write(csock, cworking, sizeof(cworking));

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
