// https://devblogs.microsoft.com/commandline/windowswsl-interop-with-af_unix/

#define WIN32_LEAN_AND_MEAN

#pragma comment(lib,"ws2_32.lib")

#include <winsock2.h>
#include <afunix.h>
#include <stdlib.h>
#include <stdio.h>

static const char * SERVER_SOCKET = "server.sock";

static void error(const char * msg)
{
    printf("ERROR: %s\n", msg);
    exit(1);
}

int main()
{
    struct addrinfo * _addressInfo;

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

    if (connect(newsock, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) {
        error("connect");
    }

    while (true) {

        double x[17] = {};
        send(newsock, (const char *)x, sizeof(x), 0);
    }

    return 0;
}
