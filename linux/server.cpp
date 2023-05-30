#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>

static const char * SERVER = "../WslLinuxSocket/server.sock";

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

    /*
     * Listen for connections
     * and send random phrase on accept
     */
    while (true) {

        break;

        //int r = rand() % num_of_lines; //Pick random phrase/hint pair
        //write(connfd, phrases[r], strlen(phrases[r]));

        close(connfd);
        sleep(1);
    }

    exit(0);
}
