#include <stdio.h>
#include <winsock2.h>

#define IP "127.0.0.1"
#define PORT_NUM 5552
#define MAXLEN 1024

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")

int main()
{
	SOCKET sockfd;
	WSADATA wsaData;
	struct sockaddr_in addr;
	int Readn, Writen;

	char buf[MAXLEN];
	char rbuf[MAXLEN];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		return 1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		return 1;
	}
	printf("Success socket create\r\n");
	memset((void *)&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(PORT_NUM);

	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return 1;
	}
	printf("Success connect\r\n");
	while (1)
	{
		printf("> ");
		fgets(buf, MAXLEN - 1, stdin);
		if (strncmp(buf, "quit\n", 5) == 0)
		{
			break;
		}

		Writen = send(sockfd, (char *)buf, strlen(buf), 0);
		memset(rbuf, 0x00, MAXLEN);
		
		Readn = recv(sockfd, (char *)buf, MAXLEN, 0);
		if(Readn > 0) {
			printf("recv : %s", buf);
		}
	}
	closesocket(sockfd);
	WSACleanup();
	return 0;
}