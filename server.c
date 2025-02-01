#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define PORT 1312

int main(){
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

    char message[254];

    recv(client, message, sizeof(message), 0);
    message[strcspn(message, "\n")] = 0;

    printf("New message received : %s\n", message);

    close(client);
    close(sk);

    return 0;
}