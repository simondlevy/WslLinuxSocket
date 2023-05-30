#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>

static const char * SERVER = "../WslLinuxSocket/server.sock";

static void report(void)
{
    struct timeval time = {};
    gettimeofday(&time, NULL);

    static uint32_t prev_sec;
    static uint32_t count;

    if (time.tv_sec != prev_sec) {
        if (count != 0) {
            printf("%3.3e Hz", (double)count);
        }
        printf("\n");
        count = 0;
        prev_sec = time.tv_sec;
    }

    count++;
}

int main(int argc, char *argv[])
{
    auto sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
        exit(-1);

    struct sockaddr_un saddr = {};
    saddr.sun_family = AF_UNIX;
    strncpy(saddr.sun_path, SERVER, sizeof(saddr.sun_path) - 1);
    if (bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
        perror("server bind error");
        exit(1);
    }

    listen(sockfd, 1);

    printf("Listening for client ...\n");

    auto connfd = accept(sockfd, NULL, NULL);

    while (true) {

        double x[17] = {};

        read(connfd, x, sizeof(x));

        report();
    }

    close(connfd);

    exit(0);
}
