// https://devblogs.microsoft.com/commandline/windowswsl-interop-with-af_unix/

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define RECVSIZE size_t
#undef TEXT

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <afunix.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <ws2tcpip.h>

#define SERVER_SOCKET "server.sock"

static void error(const char * msg)
{
    printf("ERROR: %s\n", msg);
    exit(1);
}

int main()
{
    struct addrinfo * _addressInfo;

    bool _connected;

    char _message[200];

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        error("WSAStartup");
    }

    SOCKET newsock;
    if ((newsock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        error("socket");
    }

    char sun_path[256] = {};
    sprintf_s(sun_path, "server.sock");

    SOCKADDR_UN serveraddr = {};
    serveraddr.sun_family = AF_UNIX;
    strcpy_s(serveraddr.sun_path, sun_path);

    auto error = connect(newsock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("ERROR=%d\n", error);

    return 0;
}
