// https://devblogs.microsoft.com/commandline/windowswsl-interop-with-af_unix/

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>

#define SERVER_SOCKET "../WslLinuxSocket/server.sock"
#define CLIENT_SOCKET "client.sock"
#define BIND_SOCKET "bind.sock"

static void error(const char * msg)
{
    perror(msg);
    unlink(CLIENT_SOCKET);
    exit(1);
}

static void report(void)
{
    static uint64_t count;
    static double time_prev;

    static const uint64_t MAX_COUNT = 10000;

    count++;

    if (count == MAX_COUNT) {

        struct timeval time = {};
        gettimeofday(&time, NULL);

        double time_curr = time.tv_sec + (double)time.tv_usec/1e6;

        if (time_prev > 0) {
            printf("%3.3e Hz\n", MAX_COUNT / (time_curr - time_prev));
        }

        time_prev = time_curr;
        count = 0;
    }
}


int main(int argc, char* argv[]) 
{
    int clientFd = 0;
    if ((clientFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        error("clientFd socket error");
    }

    struct sockaddr_un clientAddr = {};
    clientAddr.sun_family = AF_UNIX;
    strncpy(clientAddr.sun_path, CLIENT_SOCKET, sizeof(clientAddr.sun_path) - 1);
    if (bind(clientFd, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) == -1) {
        error("server bind error");
        unlink(CLIENT_SOCKET);
        return 0;
    }

    socklen_t addrLen = sizeof(clientAddr);
    if (getsockname(clientFd, (struct sockaddr*)&clientAddr, &addrLen) == -1) {
        error("getsockname(client)");
    }

    printf("getsockname returned: %s, addressize: %d\n", clientAddr.sun_path, addrLen);
    struct sockaddr_un serverAddr = {};
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SERVER_SOCKET, sizeof(serverAddr.sun_path));
    printf("client: connecting to %s\n", SERVER_SOCKET);
    if (connect(clientFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        error("connect error");
    }

    printf("client: connected to the server\n");
    addrLen = sizeof(clientAddr);
    if (getpeername(clientFd, (struct sockaddr*)&clientAddr, &addrLen) == -1) {
        error("getpeername(client)");
    }

    printf("getpeername returned: %s, addressize: %d\n", clientAddr.sun_path, addrLen);

    while (true) {

        char Buf[17*8] = {};

        auto BytesRecvd = recv(clientFd, Buf, sizeof(Buf), 0);

        if (BytesRecvd == -1) {
            error("recv");
        }

        if (BytesRecvd == 0) {
            break;
        }

        // printf("received: %zd bytes\n", BytesRecvd);
        report();
    }

    return 0;
}
