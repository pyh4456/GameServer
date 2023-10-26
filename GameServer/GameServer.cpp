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

void HandleError(const char* cause) {
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

const int32 BUFSIZE = 1000;

struct Session {
	SOCKET socket;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	int32 sendBytes = 0;
};

int main() {
	// WinSock 라이브러리 초기화
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// Non-Blocking
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	//Select model = (select 함수가 핵심이 된다)
	//소켓 함수 호출이 성공할 시점을 미리 알 수 있다.

	vector<Session> sessions;
	sessions.reserve(100);

	fd_set reads;
	fd_set writes;

	while (true) {
		//소켓 셋 초기화
		FD_ZERO(&reads);
		FD_ZERO(&writes);

		//ListenSocket 등록
		FD_SET(listenSocket, &reads);

		for (Session& s : sessions) {
			if (s.recvBytes <= s.sendBytes)
				FD_SET(s.socket, &reads);
			else
				FD_SET(s.socket, &writes);
		}

		int32 retVal = ::select(0, &reads, &writes, nullptr, nullptr);
		if (retVal == SOCKET_ERROR)
			break;

		//Listener 소켓 체크
		if (FD_ISSET(listenSocket, &reads)) {
			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);
			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET) {
				cout << "Client Connected" << endl;
				sessions.push_back(Session{ clientSocket });
			}
		}

		//나머지 소켓 체크
		for (Session& s : sessions) {
			//Read 체크
			if (FD_ISSET(s.socket, &reads)) {
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen <= 0) {
					//TODO : session 제거
					continue;
				}

				s.recvBytes = recvLen;
			}

			//Write 체크
			if (FD_ISSET(s.socket, &writes)) {
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR) {
					//TODO : session 제거
					continue;
				}

				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes) {
					s.recvBytes = 0;
					s.sendBytes = 0;
				}
			}
		}
	}


	//WindSock 종료
	::WSACleanup();
}