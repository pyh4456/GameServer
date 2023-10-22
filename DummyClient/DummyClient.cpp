#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause) {
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{	
	
	// WinSock 라이브러리 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		HandleError("Socket");
		return 0;
	}

	//목적지 설정
	SOCKADDR_IN serverAddr;// IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	//----------------------------
	// 연결 성공
	//----------------------------

	cout << "Connected to server!" << endl;

	while (true) {
		// TODO
		char sendBuffer[100] = "Hello World!";

		int32 errorCode = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0,
			(SOCKADDR*)&serverAddr, sizeof(serverAddr));

		if (errorCode == SOCKET_ERROR) {
			HandleError("SendTo");
			return 0;
		}

		cout << "Send Data Lend = " << sizeof(sendBuffer) << endl;
		cout << "Send Data = " << sendBuffer << endl;



		SOCKADDR_IN recvAddr;
		::memset(&recvAddr, 0, sizeof(recvAddr));
		int32 addrLen = sizeof(recvAddr);

		char recvBuffer[1000];

		int32 recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0,
			(SOCKADDR*)&recvAddr, &addrLen);

		if (recvLen <= 0) {
			HandleError("RecvFrom");
			return 0;
		}

		cout << "received data Len : " << recvLen << endl;
		cout << "received data : " << recvBuffer << endl;

		this_thread::sleep_for(1s);

	}

	//소켓 리소스 반환
	::closesocket(clientSocket);

	//WindSock 종료
	::WSACleanup();
}