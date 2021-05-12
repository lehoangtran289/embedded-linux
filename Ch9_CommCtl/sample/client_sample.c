#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_ADDR "localhost"
#define SERVER_PORT 5678

int main() {
    int iSock = 0;
    struct sockaddr_in serverData = {0, 0, ""};
    struct hostent *hp;

    // Creating socket
    if ((iSock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Getting the host of the server.
    if ((hp = gethostbyname(SERVER_ADDR)) == NULL) {
        perror("Cannot get host\n");
        shutdown(iSock, SHUT_RDWR);
        close(iSock);
        exit(1);
    }

    // Making the server identifier.
    memcpy((char *)&serverData.sin_addr, (char *)hp->h_addr, hp->h_length);
    serverData.sin_family = AF_INET;
    serverData.sin_port = SERVER_PORT;

    // Begin the connection
    if (connect(iSock, (struct sockaddr *)&serverData, sizeof(serverData)) == -1) {
        perror("connect");
        shutdown(iSock, SHUT_RDWR);
        close(iSock);
        exit(1);
    }

    // End of the connection
    puts("end of connection");
    shutdown(iSock, SHUT_RDWR);
    close(iSock);

    return 0;
}
