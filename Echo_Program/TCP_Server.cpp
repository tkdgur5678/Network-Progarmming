#include <stdio.h>
#include <winsock2.h>

#define MAX_PACKETLEN 512 
#define PORT 5552 

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int status;
	int SockLen;
	int Readn, Writen;
	SOCKET EndpointSocket, ClientSocket;
	struct sockaddr_in SockInfo, ClientSockInfo;
	char ReadBuffer[MAX_PACKETLEN];

	//����� ������ �������� WSADATA����ü�� �����͸� �־��༭ WSADATA�� ���ڿ� ���� �������� ä������.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("error\r\n");
		return 0;
	}

	//���ͳ� ���ܿ� ��ġ�� ������ ����� �ش�.
	EndpointSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (EndpointSocket == INVALID_SOCKET)
		return 1;
	printf("Success socket create\r\n");

	ZeroMemory(&SockInfo, sizeof(struct sockaddr_in));
	SockInfo.sin_family = AF_INET;
	SockInfo.sin_port = htons(PORT);
	//INADDR_ANY�� 0.0.0.0 �ּҸ� ����Ű�µ�, ��� ��밡���� �ּҷκ��� ��ٸ��ٴ� �ǹ��̴�.
	SockInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//���Ͽ� IP�� port�� �������ش�.
	status = bind(EndpointSocket, (struct sockaddr*)&SockInfo, sizeof(struct sockaddr_in));
	if (status == SOCKET_ERROR) {
		printf("Bind Error\n");
		return 0;
	}
	//Ŭ���̾�Ʈ�� ���� ���� ��⿭�� �����.(backlog�� ��⿭ũ��)
	if (SOCKET_ERROR == listen(EndpointSocket, 5)) {
		printf("listen Error\n");
		return 0;
	}
	
	while (1) {
		ZeroMemory(&ClientSockInfo, sizeof(struct sockaddr_in));
		SockLen = sizeof(struct sockaddr_in);
		//accpet�Լ��� �����⿭�� �ִ� ��û�� �����ͼ� Ŭ���̾�Ʈ�� ��ſ� ����� ��������� �����.
		ClientSocket = accept(EndpointSocket, (struct sockaddr*)&ClientSockInfo, &SockLen);
		if (ClientSocket == INVALID_SOCKET) {
			printf("Accept Error\n");
			closesocket(EndpointSocket);
			return 1;
		}
		printf("Accept Client\n");
		memset(ReadBuffer, 0x00, MAX_PACKETLEN);
		while ((Readn = recv(ClientSocket, ReadBuffer, MAX_PACKETLEN, 0)) != 0) {
			printf("recv : %s", ReadBuffer);
			if (Readn > 0) {
				Writen = send(ClientSocket, ReadBuffer, Readn, 0);
			}
			else {
				printf("read Error\n");
				break;
			}
			memset(ReadBuffer, 0x00, MAX_PACKETLEN);
		}
		closesocket(ClientSocket);
	}
	closesocket(EndpointSocket);
	WSACleanup();
	return 0;
}
