#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections */

char *strrev(char *str, int len) {
    char *p1 = str;
    char *p2 = str + len - 1;

    while (p1 < p2) {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }

    return str;
}

int main(int argc, char **argv) {
    int listenfd, connfd, n;
    pid_t pid;
    socklen_t clilen;
    char *buf = (char *)malloc(MAXLINE * sizeof(char));
    struct sockaddr_in cliaddr, servaddr;

    // creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

    /* process the request */
    char ip[100];
    inet_ntop(AF_INET, &(cliaddr.sin_addr), ip, 100);
    printf("%s %d %s\n", "Received request...", cliaddr.sin_port, ip);

    while ((n = recv(connfd, buf, MAXLINE, 0)) > 0) {
        printf("%s", "String received from:");
        puts(buf);

        if (strcmp(buf, "quit") == 0) {
            puts("end of connection");
            shutdown(connfd, SHUT_RDWR);
            close(connfd);
            shutdown(listenfd, SHUT_RDWR);
            close(listenfd);
            return 0;
        }

        buf = strrev(buf, strlen(buf));
        printf("%s", "String resend to client:");
        puts(buf);
        send(connfd, buf, n, 0);
        fflush(stdout);
        bzero(buf, 2000);
    }

    if (n < 0) {
        perror("Read error");
        exit(1);
    }

    //end of connection
    puts("end of connection");
    shutdown(connfd, SHUT_RDWR);
    close(connfd);
    shutdown(listenfd, SHUT_RDWR);
    close(listenfd);
    return 0;
}
