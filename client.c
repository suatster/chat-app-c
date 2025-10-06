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
int receive_message(int sockfd, char* str_to_save);

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

    char* buffer = malloc(50*sizeof(char));
    receive_message(socketfd, buffer);
    printf("received: %s\n", buffer);

    printf("Disconnecting...\n");
    close(socketfd);
    printf("Disconnected!\n");
    return 0;
}

int send_message(int target_fd, char* msg){
    //sends the message prefixed with message length.
    
    int len = strlen(msg);
    int32_t net_len = htonl(len);

    int bytes_sent = 0;
    while(bytes_sent < 4){
        int sent_at_once = send(target_fd, ((uint8_t*)&net_len) + bytes_sent, 4 - bytes_sent, 0);
        if(sent_at_once <= 0) return -1;
        bytes_sent += sent_at_once;
    }

    // Send the message body
    if (send(target_fd, msg, len, 0) != len) {
        return 0; // failed
    }

    return 1; // success
}

int32_t receive_message_length(int receivefd){
    int bytes_read = 0;
    int32_t len = 0;
    while(bytes_read < 4){
        int read_at_once = recv(receivefd, ((uint8_t*)&len)+bytes_read, 4 - bytes_read, 0);
        if(read_at_once <= 0) {
            perror("recv error");
            return -1;
        }
        bytes_read += read_at_once;
    }

    len = ntohl(len);
    return len;
}

int receive_message(int receivefd, char* buffer){
    int32_t message_length = receive_message_length(receivefd);

    if(message_length <= 0){
        perror("receive length error");
        return -1;
    }

    int bytes_read = 0;
    while(bytes_read < message_length){
        int read_at_once = recv(receivefd, (uint8_t*)buffer+bytes_read, message_length - bytes_read, 0);
        if(read_at_once <= 0){
            perror("recv error");
            return -1;
        }
        bytes_read += read_at_once;
    }
    buffer[message_length] = '\0';
    
    return 1;
}

