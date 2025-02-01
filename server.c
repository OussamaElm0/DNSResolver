#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 1312

int main(){
    char message_recv[254], official_address[254];
    int sk = socket(AF_INET, SOCK_STREAM, 0);

    if(sk < 0){
        perror("Error in creating socket");
        return 1;
    }

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

    printf("The server is running on port %d...\n", PORT);

    int client = accept(sk, NULL, NULL);

    if (client < 0){
        perror("Accept error");
        return 1;
    }

    recv(client, message_recv, sizeof(message_recv), 0);
    message_recv[strcspn(message_recv, "\n")] = 0;

    printf("New message received : %s\n", message_recv);

    struct hostent *host = gethostbyname(message_recv);


    strcpy(official_address,(char *) inet_ntoa(*(struct in_addr *) host->h_addr));

    send(client, official_address, sizeof(official_address), 0);

    printf("The official address is : %s\n", official_address);

    close(client);
    close(sk);

    return 0;
}