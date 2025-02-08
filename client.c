#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORT 1312

void displayOptions();
void getIp(int sk);
void getDomainName(int sk);

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
    printf("Welcome!\n");
    char option[15];

    while (1){
        memset(option, 0, sizeof(option));

        displayOptions();
        printf("Enter option: ");
        fgets(option, sizeof(option), stdin);
        option[strcspn(option, "\n")] = 0;

        if(strcmp(option, "/exit") == 0){
            printf("Disconnected from server.\n");
            break;
        } else if(strcmp(option, "/ip") == 0) {
            send(sk, option, strlen(option), 0);
            getIp(sk);
        } else if(strcmp(option, "/dname") == 0){
            send(sk, option, strlen(option), 0);
            getDomainName(sk);
        } else {
            printf("Sorry! Unavailable option.\n");
        }
    }

    close(sk);
    return 0;
}

void displayOptions(){
    printf(
        "===============================================\n"
        "Resolve address ip from domain name: /ip\n"
        "Resolve domain name from address ip: /dname\n"
        "To exit: /exit\n"
        "===============================================\n"
    );
}

void getIp(int sk){
    char message[254], message_recv[254];

    printf("Enter the domain name: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;
    printf("Message entered: %s\n", message);

    send(sk, message, strlen(message), 0);
    printf("Message sent successfully!\n");

    recv(sk, message_recv, sizeof(message_recv), 0);
    message_recv[strcspn(message_recv, "\n")] = 0;

    printf("--------------------------------------------------------------------------------\n");
    printf("Response from the server: %s\n", message_recv);
    printf("--------------------------------------------------------------------------------\n");
}

void getDomainName(int sk){
    char ip_address[254], response[1024];
    memset(response, 0, sizeof(response));

    printf("Enter the address ip: ");
    fgets(ip_address, sizeof(ip_address), stdin);
    ip_address[strcspn(ip_address, "\n")] = 0;
    printf("Ip address entered: %s\n", ip_address);

    send(sk, ip_address, strlen(ip_address), 0);
    printf("Message sent successfully!\n");

    recv(sk, response, sizeof(response), 0);

    printf("--------------------------------------------------------------------------------\n");
    printf("Response from the server: \n%s\n", response);
    printf("--------------------------------------------------------------------------------\n");
}