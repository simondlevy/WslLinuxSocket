#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    /*
    * Create Sockets
    */
    auto listfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listfd == -1)
        exit(-1);

    struct sockaddr saddr = {AF_UNIX, "server.sock"};
    socklen_t saddrlen = sizeof(struct sockaddr) + 6;
    bind(listfd, &saddr, saddrlen);

    listen(listfd, 10);

    fflush(stdout);
    printf("Running...\n");

    /*
    * Listen for connections
    * and send random phrase on accept
    */
    while (true) {

        auto connfd = accept(listfd, NULL, NULL);

        //int r = rand() % num_of_lines; //Pick random phrase/hint pair
        //write(connfd, phrases[r], strlen(phrases[r]));

        close(connfd);
        sleep(1);
    }

    exit(0);
}
