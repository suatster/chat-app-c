/*INCLUDE*/
//generic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//networking
#include <arpa/inet.h> //inet_ntoa()
#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in

//read() and write()
#include <unistd.h>


/*DEFINE*/
#define PORT 8080

/*PROTOTYPES*/
char* escape_characters(char* input);


int main(){
    /*SET SERVER*/
    //Open the socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(socketfd < 0){ // Failed?
        perror("Couldn't set socketfd.");
        exit(EXIT_FAILURE);
    }
    printf("socketfd successful!\n");

    //Setting the structs
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); //reset the full struct
    server_addr.sin_family = AF_INET; //protocol, currently set to ipv4
    server_addr.sin_port = htons(PORT); //port to listen
    server_addr.sin_addr.s_addr = INADDR_ANY; //addresses to accept
    printf("Set structs successfully!\n");

    //bind the socket
    if(bind(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        //failed
        perror("Failed to bind the socket.");
        exit(EXIT_FAILURE);
    }
    printf("Socket binding successful!\n");


    /*LISTEN*/
    //time to listen, client is always right after all ¯\_(ツ)_/¯
    if(listen(socketfd, 5) < 0){
        //cant listen.
        perror("Failed to listen!");
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d.\n", PORT);


    /*SET CLIENT*/
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int clientfd = accept(socketfd, (struct sockaddr*)&client_addr, &client_addr_size);

    if(clientfd < 0){ //Failed?
        perror("accept() failed.");
        exit(EXIT_FAILURE);
    }
    printf("Client connected!\n");


    //receive a message
    char *buffer = malloc(50*sizeof(char));
    int bytes_read;

    while((bytes_read = read(clientfd, buffer, sizeof(buffer))) > 0){
        printf("%s said: %s\n", inet_ntoa(client_addr.sin_addr), buffer);
        char* clean_msg = escape_characters(buffer);
        send(clientfd, clean_msg, strlen(clean_msg), 0);
        free(clean_msg);
    }
    printf("Client disconnected!\n");

    close(socketfd);
    return 0;
}


char* escape_characters(char* input){
    char* output = malloc(2 * strlen(input) + 1); //worst case, all need to be escaped
    char* start = output; //keep track of the beginning
    while(*input){
        switch(*input){
            case '\n':
                *output++ = '\\';
                *output++ = 'n';
                break;
            case '\t':
                *output++ = '\\';
                *output++ = 't';
                break;
            case '\\':
                *output++ = '\\';
                *output++ = '\\';
                break;
            default:
                *output++ = *input;
        }
        input++;
    }

    *output = '\0';
    return start;
}

