#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORT 1312

int main(){
    int sk = socket(AF_INET, SOCK_STREAM, 0);

    if(sk < 0 ){
        perror("Socket creation error");
        return 1;
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = PORT;
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(connect(sk, (struct sockaddr *)&server_address, sizeof(server_address)) < 0 ){
        perror("Connection failed");
        return 1;
    }

    char message[254];

    printf("Enter the message to send : \n");
    fgets(message, sizeof(message), stdin);

    send(sk, message, strlen(message), 0);

    printf("Your message sent successfully!\n");

    char message_recv[254];

    recv(sk, message_recv, sizeof(message_recv), 0);

    printf("The official address of %s is : %s\n", message, message_recv);

    close(sk);


    return 0;
}