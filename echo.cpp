#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsadata;
    SOCKET listenSock, clientSock;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // win소켓 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        cerr << "WSAStartup() failed" << endl;
        return 1;
    }

    // 소켓 생성
    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET)
    {
        cerr << "socket() failed" << endl;
        WSACleanup();
        return 1;
    }

    // 바인딩
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cerr << "bind() failed" << endl;
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    // 리슨 상태로 전환
    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
    {
        cerr << "listen() failed" << endl;
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }
    cout << "Echo 서버 리슨 중(port: 9000)" << endl;

    // 클라 접속 대기
    clientSock = accept(listenSock, (SOCKADDR *)&clientAddr, &clientAddrSize);
    if (clientSock == INVALID_SOCKET)
    {
        cerr << "accept() failed" << endl;
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }
    cout << "Client connected!" << endl;

    // 데이터 송수신
    char buffer[512];
    int recvLen;
    while ((recvLen = recv(clientSock, buffer, sizeof(buffer), 0)) > 0)
    {
        buffer[recvLen] = '\0';
        cout << "Received: " << buffer << endl;
        send(clientSock, buffer, recvLen, 0);
    }

    // 연결 종료
    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}