#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFF_SIZE 8192

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Wrong arguments | %s <Server IP> <Port>\n", argv[0]);
        exit(1);
    }

    // device info
    char name[50] = "";
    int mode_2 = 4700;
    int mode_3 = 1000;
    int use_mode = 0;

    // device info
    // char name[50];
    // int mode_2;
    // int mode_3;
    // int use_mode;
	printf("--- POWER SUPPLY PROGRAM ---\n");
    printf("Enter device's name: ");
    //sscanf(stdin, "%*c%^[\n]", name);
    //scanf("%s", name);
    gets(name);
    printf("Enter normal power mode: ");
    scanf("%d", &mode_2);
    getchar();
    printf("Enter limited power mode: ");
    scanf("%d", &mode_3);
    getchar();
	printf("-----------------------------\n\n\n");

    // Init variable
    int client_sock;
    char buff[BUFF_SIZE];
    struct sockaddr_in server_addr;
    int msg_len, bytes_sent, bytes_received;

    // Construct socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Request to connect server
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        perror("connect failed\n");
        exit(1);
    }

    // Communicate with server

    // send device info to server
    memset(buff, '\0', strlen(buff) + 1);
    sprintf(buff, "n%s|%d|%d", name, mode_2, mode_3);
    msg_len = strlen(buff);
    if (msg_len == 0) {
        printf("No device info found\n");
        close(client_sock);
        exit(1);
    }
    bytes_sent = send(client_sock, buff, msg_len, 0);
    if (bytes_sent <= 0) {
        printf("Connection close\n");
        close(client_sock);
        exit(1);
    }

    // wait for server response
    if (fork() == 0) {
        // Child: listen from server
        while (1) {
            bytes_received = recv(client_sock, buff, BUFF_SIZE - 1, 0);
            if (bytes_received <= 0) { // if DISCONNECT
                printf("\nServer shuted down. Exiting...\n");
                break;
            } else {
                buff[bytes_received] = '\0';
            }

            int buff_i = atoi(buff);
            // if (buff_i = 9) => max device reached => quit

            if (buff_i == 9) {
                printf("Max devices reached. Cannot continue connecting\n");
            }
        }
    } else { // Parent: open menu
        do {
            sleep(1);
			printf("------ DEVICE %s MENU -----------\n", name);
			printf("| 0. Turn off device                |\n");
			printf("| 1. Normal mode                    |\n");
			printf("| 2. Power saving mode              |\n");
			printf("| Other. Disconnect device          |\n");
			printf("------------------------------------\n");
			printf("Your selection: ");

            char menu = getchar();
            getchar();
            char rq[3] = "mxx";
            switch (menu) {
                case '0':
                    printf("\n-> TURN OFF DEVICE %s\n\n", name);
                    break;
                case '1':
                    printf("\n-> DEVICE %s: NORMAL MODE\n\n", name);
                    break;
                case '2':
                    printf("\n-> DEVICE %s: POWER SAVING MODE\n\n", name);
                    break;
                default:
                    menu = '3';
                    printf("\n-> DEVICE %s: DISCONNECTED\n", name);
            }
            rq[1] = menu;
            rq[2] = '\0';
            send(client_sock, rq, 1, 0);
            if (menu == '3')
                break;
        } while (1);
    }

    // Close socket
    close(client_sock);
    kill(0, SIGKILL);
    return 0;
}