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

	//사용할 윈속의 버전값과 WSADATA구조체의 포인터를 넣어줘서 WSADATA형 인자에 세부 정보들이 채워진다.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("error\r\n");
		return 0;
	}

	//인터넷 종단에 위치할 소켓을 만들어 준다.
	EndpointSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (EndpointSocket == INVALID_SOCKET)
		return 1;
	printf("Success socket create\r\n");

	ZeroMemory(&SockInfo, sizeof(struct sockaddr_in));
	SockInfo.sin_family = AF_INET;
	SockInfo.sin_port = htons(PORT);
	//INADDR_ANY는 0.0.0.0 주소를 가리키는데, 모든 사용가능한 주소로부터 기다린다는 의미이다.
	SockInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//소켓에 IP와 port를 지정해준다.
	status = bind(EndpointSocket, (struct sockaddr*)&SockInfo, sizeof(struct sockaddr_in));
	if (status == SOCKET_ERROR) {
		printf("Bind Error\n");
		return 0;
	}
	//클라이언트의 연결 받을 대기열을 만든다.(backlog가 대기열크기)
	if (SOCKET_ERROR == listen(EndpointSocket, 5)) {
		printf("listen Error\n");
		return 0;
	}
	
	while (1) {
		ZeroMemory(&ClientSockInfo, sizeof(struct sockaddr_in));
		SockLen = sizeof(struct sockaddr_in);
		//accpet함수로 연결대기열에 있는 요청을 가져와서 클라이언트와 통신에 사용할 연결소켓을 만든다.
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
