#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    // basic check of the arguments additional checks can be inserted
    if (argc != 2) {
        perror("Usage: TCPClient <IP address of the server");
        exit(1);
    }

    // Create a socket for the client
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    // Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT);  // convert to big-endian order

    // Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Problem in connecting to the server");
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
        exit(3);
    }

    int choice = 0;
    char sendline[1000], recvline[1000];
    do {
        printf("\n-------\n1. Process to server\n");
        printf("2. Exit\n");
        printf("Input a choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                strcpy(sendline, "");
                strcpy(recvline, "");
                printf("\nEnter a string: ");
                fgets(sendline, MAXLINE, stdin);
                if (sendline[0] == '\n') {
                    puts("Invalid string\n");
                    break;
                }
                if ((strlen(sendline) > 0) && (sendline[strlen(sendline) - 1] == '\n')) {
                    sendline[strlen(sendline) - 1] = '\0';
                }
                send(sockfd, sendline, strlen(sendline), 0);

                if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
                    perror("The server terminated prematurely");
                    shutdown(sockfd, SHUT_RDWR);
                    close(sockfd);
                    exit(4);
                }
                printf("%s", "String received from the server: ");
                fputs(recvline, stdout);

                break;

            case 2:
                printf("Exit\n");
                exit(0);
                break;

            default:
                printf("Invalid task Try again!\n\n");
                break;
        }
    } while (choice != 2);

    exit(0);
}