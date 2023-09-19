#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
	// WinSock 라이브러리 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		return 0;
	}

	//목적지 설정
	SOCKADDR_IN serverAddr;// IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);//알아서 연결해달라는 의미
	serverAddr.sin_port = ::htons(7777);

	//listener 소켓 준비
	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr))){
		int32 errCode = ::WSAGetLastError();
		cout << "Bind ErrorCode : " << errCode << endl;
		return 0;
	}

	//listen
	if (::listen(listenSocket, 10)) { //대기열의 한도 설정 (현재 10)
		int32 errCode = ::WSAGetLastError();
		cout << "Listen ErrorCode : " << errCode << endl;
		return 0;
	}

	//연결 처리
	// ----------------------------
	while (true) {
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET) {
			int32 errCode = ::WSAGetLastError();
			cout << "Accept ErrorCode : " << errCode << endl;
			return 0;
		}

		// accept
		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Connected! IP = " << ipAddress << endl;
		//TODO
	}
	// ----------------------------


	//WindSock 종료
	::WSACleanup();
}