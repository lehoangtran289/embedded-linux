#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_ADDR “localhost”
#define SERVER_PORT 5678

int main() {
    int iSock = 0;
    int iClientSock = 0;
    struct sockaddr_in serverData = {0, 0, ""};
    struct sockaddr_in clientData = {0, 0, ""};

    int iClientSize = 0;

    // Creation of socket
    if ((iSock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    // Identifier making
    serverData.sin_family = AF_INET;
    serverData.sin_addr.s_addr = INADDR_ANY;
    serverData.sin_port = SERVER_PORT;

    // Identify the socket
    if (bind(iSock, (struct sockaddr*)&serverData, sizeof(serverData)) == -1) {
        perror("bind");
        shutdown(iSock, SHUT_RDWR);
        close(iSock);
        exit(1);
    }

    // Connection setup
    if (listen(iSock, 1) == -1) {
        perror("listen");
        shutdown(iSock, SHUT_RDWR);
        close(iSock);
        exit(1);
    }

    iClientSize = sizeof(clientData);
    // Connection standby
    if ((iClientSock = accept(iSock, (struct sockaddr*)&clientData, &iClientSize)) == -1) {
        perror("accept");
        shutdown(iSock, SHUT_RDWR);
        close(iSock);
        exit(1);
    }
    //end of connection
    puts("end of connection");
    shutdown(iClientSock, SHUT_RDWR);
    close(iClientSock);
    shutdown(iSock, SHUT_RDWR);
    close(iSock);
    return 0;
}
