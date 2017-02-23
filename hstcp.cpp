#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define PORT 4

#define ECHO 0
#define BUY 1
#define SELL 2
#define X 3

char *env;
char port[255];

void MakePath(char* filename, char *ret) {
	strcpy(ret, env);
	strcat(ret, "\\..\\");
	strcat(ret, port);
	strcat(ret, "\\");
	strcat(ret, filename);
}

void InputServername(char* arc) {
	int psi = strlen(arc);
	int pi = 0;
	for (int i = 0; i < strlen(arc); i++) {
		if (arc[i] == ':') {
			psi = i;
		}
		if (psi < i) {
			port[pi++] = arc[i];
		}
	}
	port[pi] = 0;
	arc[psi] = 0;
}

int __cdecl main(int argc, char **argv)
{
	env = getenv("HSWORD_HOME");
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char sendbuf[512];
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	bool A = false;
	int type = 0;

	// Validate the parameters
	if (argc < 2) {
		printf("HSword TCP Client v1.0.2.23\n");
		printf("usage: %s server-name [-e] [-s ID Price Count] [-b ID Price Count] [-x ID]\n", argv[0]);
		return 1;
	}

	for (int i = 2; i < argc; i++)
	{
		if (argv[i][1] == 'e')
		{
			A = true;
			type = ECHO;
		}
		if (argv[i][1] == 's')
		{
			A = true;
			type = SELL;
		}
		if (argv[i][1] == 'b')
		{
			A = true;
			type = BUY;
		}
		if (argv[i][1] == 'x')
		{
			A = true;
			type = X;
		}
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	InputServername(argv[1]);
	if (strlen(port) != PORT)
		return 1;
	iResult = getaddrinfo(argv[1], port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}




	while (true) {
		if (!A) {
			printf("CMD>");
			fgets(sendbuf, DEFAULT_BUFLEN, stdin);
			char *p;
			if ((p = strchr(sendbuf, '\n')) != NULL) *p = '\0';
			if (sendbuf[0] == '-')
				break;
		}
		else {
			if(type==ECHO)
				strcpy(sendbuf,"echo");
			else if (type == BUY)
			{
				if (argc <= 5)
					return 1;
				strcpy(sendbuf, "buy ");
				strcat(sendbuf, argv[3]);
				strcat(sendbuf, " ");
				strcat(sendbuf, argv[4]);
				strcat(sendbuf, " ");
				strcat(sendbuf, argv[5]);
			}
			else if (type == SELL)
			{
				if (argc <= 5)
					return 1;
				strcpy(sendbuf, "sell ");
				strcat(sendbuf, argv[3]);
				strcat(sendbuf, " ");
				strcat(sendbuf, argv[4]);
				strcat(sendbuf, " ");
				strcat(sendbuf, argv[5]);
			}
			else if (type == X) {
				if (argc <= 3)
					return 1;
				strcpy(sendbuf, "x ");
				strcat(sendbuf, argv[3]);
			}
		}
		

		iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		recvbuf[iResult] = 0;
		
		if (iResult > 0) {
			printf("Bytes received: %d[%s]\n", iResult, recvbuf);
			if (type == X) {
				char file[255];
				MakePath("x.txt",file);
				FILE *out = fopen(file, "w");
				fprintf(out,"%s",recvbuf);
				fclose(out);
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

		if (A)
			break;
	}

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
	printf("shutdown failed with error: %d\n", WSAGetLastError());
	closesocket(ConnectSocket);
	WSACleanup();
	return 1;
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}