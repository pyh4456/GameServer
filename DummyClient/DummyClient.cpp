#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{	
	
	// WinSock 라이브러리 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	//목적지 설정
	SOCKADDR_IN serverAddr;// IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);
	
	//연결 시도
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr))) {
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	//----------------------------
	// 연결 성공
	//----------------------------

	cout << "Connected to server!" << endl;

	while (true) {
		this_thread::sleep_for(1s);
	}

	//소켓 리소스 반환
	::closesocket(clientSocket);

	//WindSock 종료
	::WSACleanup();
}