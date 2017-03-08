#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "swordtest2Dlg.h"



// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512

class Cswordtest2Dlg;
class HSOCKET {

private :
	Cswordtest2Dlg* dlg;
public :
	char servermsg[100];
	char port[255];

	void SetPort(char* s) {
		strcpy(port, s);
	}

	void SetDlg(Cswordtest2Dlg* _d) {
		dlg = _d;
	}
	void (Cswordtest2Dlg::*ptr_buy)(CString i, CString p,CString cnt);
	void (Cswordtest2Dlg::*ptr_sell)(CString i, CString p,CString cnt);
	void (Cswordtest2Dlg::*ptr_x)(CString i);
	int run(void) {
		WSADATA wsaData;
		int iResult;

		SOCKET ListenSocket = INVALID_SOCKET;
		SOCKET ClientSocket = INVALID_SOCKET;

		struct addrinfo *result = NULL;
		struct addrinfo hints;

		int iSendResult;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		printf("#%s\n",port);
		iResult = getaddrinfo(NULL, port, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		// Create a SOCKET for connecting to server
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}
		char nSockOpt = 1;
		setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt));
		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		freeaddrinfo(result);

		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		printf(">Listening...\n");

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		printf(">Accept.\n");
		// No longer need server socket
		closesocket(ListenSocket);
		


		// Receive until the peer shuts down the connection
		do {

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {

				recvbuf[iResult] = 0;
				CommandProcessor(recvbuf);

				iSendResult = send(ClientSocket, recvbuf, strlen(recvbuf), 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}
				printf("Bytes sent: %d[%s]\n", iSendResult, recvbuf);
			}
			else if (iResult == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 0;
			}

		} while (iResult > 0);

		// shutdown the connection since we're done
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

		// cleanup
		closesocket(ClientSocket);
		WSACleanup();
		return 0;
	}

	char* CommandProcessor(char* str) {
		if (str[0] == 'x')
		{
			int idx = 0;
			char id[10];
			for (int i = 2; i < strlen(str); i++)
			{
				id[idx++] = str[i];
			}
			id[idx] = 0;
			memset(servermsg, 0, sizeof(servermsg));
			x(id);
			for (int i = 0; i < 2000; i++) {
				if (!strcmp(servermsg, "")) {
					printf(".");
					continue;
				}
				break;
			}
			printf("\n");
			if (!strcmp(servermsg, ""))
				strcpy(servermsg, "0 0 0");
			printf("%s\n",servermsg);
			memset(str,0, sizeof(str));
			strcpy(str, servermsg);
		}
		else if (str[0] == 'e') {
			strcpy(str, "Connection state is good.");
		}
		else if (str[0] == 'b') {
			char id[7];
			char price[10];
			char cnt[10];
			int idx = 0;
			for (int i = 4; i < strlen(str); i++)
			{
				if (str[i] == ' ')break;
				id[idx++] = str[i];
			}
			id[idx] = 0;
			idx = 0;
			for (int i = 11; i < strlen(str); i++)
			{
				if (str[i] == ' ')
					break;
				price[idx++] = str[i];
			}
			price[idx] = 0;
			idx = 0;
			for (int i = strlen(id) + strlen(price) + 2 +4; i < strlen(str); i++)
			{
				cnt[idx++] = str[i];
			}
			cnt[idx] = 0;
			buy(id, price, cnt);
			strcpy(str, "Buy request is complete!");
		}
		else if (str[0] == 's') {
			char id[7];
			char price[10];
			char cnt[10];
			int idx = 0;
			for (int i = 5; i < strlen(str); i++)
			{
				if (str[i] == ' ')break;
				id[idx++] = str[i];
			}
			id[idx] = 0;
			idx = 0;
			for (int i = 12; i < strlen(str); i++)
			{
				if (str[i] == ' ')
					break;
				price[idx++] = str[i];
			}
			price[idx] = 0;
			idx = 0;
			for (int i = strlen(id) + strlen(price) +2+ 5; i < strlen(str); i++)
			{
				cnt[idx++] = str[i];
			}
			cnt[idx] = 0;
			sell(id, price,cnt);
			strcpy(str, "Sell request is complete!");
		}
		else {
			strcpy(str, "Request is error.");
		}
		return str;
	}

	void buy(char* id, char* price,char* cnt)
	{
		printf("[종목: %s / 매수가: %s / 수량: %s]\n", id, price,cnt);
		CString _i, _p,_c;
		_i = id;
		_p = price;
		_c = cnt;
		(dlg->*ptr_buy)(_i, _p, _c);
	}

	void sell(char* id, char* price,char* cnt)
	{
		printf("[종목: %s / 매도가: %s / 수량: %s]\n", id, price,cnt);
		CString _i, _p,_c;
		_i = id;
		_p = price;
		_c = cnt;
		(dlg->*ptr_sell)(_i, _p, _c);
	}

	void x(char* id) {
		CString _i;
		_i = id;
		(dlg->*ptr_x)(_i);
	}
};