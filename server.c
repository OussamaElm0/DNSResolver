#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>

#define PORT 1312

int main(){
    char message_recv[254], official_address[254];
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

    printf("erver is running and listening on port %d.\n", PORT);


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
                memset(message_recv, 0, sizeof(message_recv));
                int read = recv(client, message_recv, sizeof(message_recv) - 1, 0);
                message_recv[strcspn(message_recv, "\n")] = 0;

                if((read < 0) || (strlen(message_recv) == 0)){
                    close(client);
                    break;
                } else {
                    printf("New domain query received: %s\n", message_recv);

                    struct hostent *host = gethostbyname(message_recv);

                    strcpy(official_address,(char *) inet_ntoa(*(struct in_addr *) host->h_addr));
                    send(client, official_address, sizeof(official_address), 0);

                    printf("Official address resolved: %s\n", official_address);  
                }
            }

            printf("PID: %d\n", getpid());
            exit(0);
        }
    }

    close(sk);

    return 0;
}