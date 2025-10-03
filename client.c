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
    if(inet_pton(AF_INET, TARGET_IP_STR, &server_addr.sin_addr.s_addr) < 0){
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

    //send a message
    char* msg = "Hello from client!";
    send(socketfd, msg, strlen(msg), 0);

    char buffer[1024];
    int n = recv(socketfd, buffer, sizeof(buffer)-1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Server replied: %s\n", buffer);
    }



    printf("Connected to server!\n");
    printf("Disconnecting...\n");
    close(socketfd);
    printf("Disconnected!\n");
    return 0;
}

