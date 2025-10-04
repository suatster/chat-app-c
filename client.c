/*INCLUDE*/
//generic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//networking
#include <arpa/inet.h> //inet_pton()
#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in

//read(), write(), close()...
#include <unistd.h>

/*DEFINE*/
#define TARGET_IP_STR "127.0.0.1"
#define TARGET_PORT 8080

int send_message(int sockfd, char* msg);
int receive_message(int sockfd, char* str_to_save, size_t str_size);

int main(){
    //open socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(socketfd < 0){
        perror("Couldn't set socketfd.\n");
        exit(EXIT_FAILURE);
    }
    printf("Set sockfd!\n");
    
    //setting the structs, target address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TARGET_PORT);
    if(inet_pton(AF_INET, TARGET_IP_STR, &server_addr.sin_addr.s_addr) <= 0){ // 0 means invalid format
        fprintf(stderr, "Invalid or unsupported address: %s\n", TARGET_IP_STR);
        exit(EXIT_FAILURE);
    }
    printf("Set structs successfully!\n");

    //time to feel a deep connection ^_^ 
    printf("Connecting to %s:%d\n", TARGET_IP_STR, TARGET_PORT);
    if (connect(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        fprintf(stderr, "Couldn't connect to: %s:%d\n", TARGET_IP_STR, TARGET_PORT); //maybe not (ಥ_ಥ)
        close(socketfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server!\n");

    send_message(socketfd, "i <3 coding at 2am.");

    printf("Disconnecting...\n");
    close(socketfd);
    printf("Disconnected!\n");
    return 0;
}

int send_message(int sockfd, char* msg){
    //sends the message prefixed with message length.
    
    int len = strlen(msg);
    int32_t net_len = htonl(len);

    int bytes_sent = 0;
    while(bytes_sent < 4){
        int sent_at_once = send(sockfd, ((uint8_t*)&net_len) + bytes_sent, 4 - bytes_sent, 0);
        if(sent_at_once <= 0) return -1;
        bytes_sent += sent_at_once;
    }

    // Send the message body
    if (send(sockfd, msg, len, 0) != len) {
        return 0; // failed
    }

    return 1; // success
}

int receive_message(int sockfd, char* str_to_save, size_t str_size){
    int n = recv(sockfd, str_to_save, str_size, 0);
    if (n > 0) {
        str_to_save[n] = '\0';
        return 1;
    }
    else return 0;
}

