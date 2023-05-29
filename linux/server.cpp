// https://devblogs.microsoft.com/commandline/windowswsl-interop-with-af_unix/

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#define CLIENT_SOCKET "client.sock"
#define SERVER_SOCKET "./server.sock"

int main(int argc, char* argv[]) 
{
    char Buf[100] = { 0 };
    ssize_t BytesRecvd = 0;
    struct sockaddr_un clientAddr = { 0 };
    struct sockaddr_un serverAddr = { 0 };
    int clientFd = 0;
    socklen_t addrLen = 0;

    if ((clientFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("clientFd socket error");
        goto ErrorExit;
    }

    clientAddr.sun_family = AF_UNIX;
    strncpy(clientAddr.sun_path, CLIENT_SOCKET, sizeof(clientAddr.sun_path) - 1);
    if (bind(clientFd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == -1) {
        perror("server bind error");
        goto ErrorExit;
    }

    addrLen = sizeof(clientAddr);
    if (getsockname(clientFd, (struct sockaddr*)&clientAddr, &addrLen) == -1) {
        perror("getsockname(client)");
        goto ErrorExit;
    }

    printf("getsockname returned: %s, addressize: %d\n", clientAddr.sun_path, addrLen);
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SERVER_SOCKET, sizeof(serverAddr.sun_path));
    printf("client: connecting to %s\n", SERVER_SOCKET);
    if (connect(clientFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("connect error");
        goto ErrorExit;
    }

    printf("client: connected to the server\n");
    addrLen = sizeof(clientAddr);
    if (getpeername(clientFd, (struct sockaddr*)&clientAddr, &addrLen) == -1) {
        perror("getpeername(client)");
        goto ErrorExit;
    }

    printf("getpeername returned: %s, addressize: %d\n", clientAddr.sun_path, addrLen);
    if ((BytesRecvd = recv(clientFd, Buf, sizeof(Buf), 0)) == -1) {
        perror("recv");
        goto ErrorExit;
    }

    printf("received: %zd bytes, %s\n", BytesRecvd, Buf);

ErrorExit:
    unlink(CLIENT_SOCKET);
    return 0;
}
