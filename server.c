#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 1312

void handleIpAddress(int client);

int main(){
    char option[254];
    int sk = socket(AF_INET, SOCK_STREAM, 0);

    if(sk < 0){
        perror("Error in creating socket");
        return 1;
    }

    // Reap zombie processes:
    signal(SIGCHLD, SIG_IGN);

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = PORT;
    address.sin_addr.s_addr = INADDR_ANY;

    if(bind(sk, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Bind error");
        close(sk);
        return 1;
    }

    if (listen(sk, 3) < 0){
        perror("Listen error");
        close(sk);
        return 1;
    }
    printf("Server is running and listening on port %d.\n", PORT);

    while(1){
        int client = accept(sk, NULL, NULL);
        pid_t processType = fork();
        
        if (client < 0){
            perror("Accept error");
            return 1;
        }

        if(processType > 0) {
            close(client);
        } else if (processType == 0){
            close(sk);
            while(1){
                memset(option, 0, sizeof(option));
                int read = recv(client, option, sizeof(option) - 1, 0);

                printf("Option : %s\n", option);
                if((read < 0) || (strlen(option) == 0)){
                    close(client);
                    break;
                } else if(strcmp(option, "/ip") == 0) {
                    handleIpAddress(client);
                }
            }
            printf("Client on processus %d disconnected.\n", getpid());
            exit(0);
        }
    }

    close(sk);
    return 0;
}

void handleIpAddress(int client){
    char official_address[254], domaine_name[254];
    memset(domaine_name, 0, sizeof(domaine_name));
    recv(client, domaine_name, sizeof(domaine_name) - 1, 0);
    printf("New domain query received: %s\n", domaine_name);
    struct hostent *host = gethostbyname(domaine_name);

    if(host != NULL){
        strcpy(official_address,(char *) inet_ntoa(*(struct in_addr *) host->h_addr));
        printf("Address : %s\n", official_address);
        
        send(client, official_address, sizeof(official_address), 0);
        printf("Official address resolved: %s\n", official_address);  
    } else {
        char *error_message = "Something went wrong! Invalid address.";
        send(client, error_message, strlen(error_message), 0);
        printf("Error message sent successfully\n");
    }                    
}