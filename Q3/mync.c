#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void run_prog(char* E) {
    // Count the number of tokens and store them
    int count = 0;
    char* tokens[256];
    char* token = strtok(E, " ");
    while (token != NULL) {
        tokens[count] = token;
        printf("%d: %s\n", count, tokens[count]);
        count++;
        token = strtok(NULL, " ");
    }
    // Allocate an array for the arguments
    char** args = malloc(sizeof(char*) * (count + 1));

    // Fill the arguments array with the tokens
    for (int i = 0; i < count; i++) {
        args[i] = tokens[i];
    }

    // The last element of the arguments array must be NULL
    args[count] = NULL;

    // Execute the program
    execvp(args[0], args);
    perror("execvp fail");
    free(args);
}

int TCP_CLIENT_PORT(char *ip, int port){
    int sockfd;

    // open a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("TCP Socket created\n");

    // connect to the server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server\n");

    return sockfd;


}

int TCP_SERVER_PORT(int port) {
    int sockfd;

    // open a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("TCP Socket created\n");
    // set the socket to reuse the address
    int optval;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    // bind the socket to the address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("Socket bound to address\n");

    // listen for incoming connections
    if (listen(sockfd, 1) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Listening for incoming connections\n");

    // accept an incoming connection
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd;

    if ((client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
        perror("accept");
        exit(1);
    }

    printf("Accepted incoming connection\n");

    return client_sockfd;
}

int main(int argc, char* argv[]) {
    char* E = NULL;
    char* O = NULL;
    char* I = NULL;
    char* B = NULL;

    int opt;

    int inputFD = STDIN_FILENO;
    int outputFD = STDOUT_FILENO;

    while ((opt = getopt(argc, argv, "e:i:b:o:")) != -1) {
        switch (opt) {
            case 'e':
                E = optarg;
                break;
            case 'o':
                O = optarg;
            break;
            case 'i':
                I = optarg;
            break;
            case 'b':
                B = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-e string]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (O != NULL) {
        if (strncmp(O, "TCPS", 4) == 0) {
        O += 4;
        int port = atoi(O);
        outputFD = TCP_SERVER_PORT(port);
    } else if (strncmp(O, "TCPC", 4) == 0) {
        O += 4;
        char *ip = strtok(O, ",");
        char *port_str = strtok(NULL, ",");
        int port = atoi(port_str);
        outputFD = TCP_CLIENT_PORT(ip, port);
    }

    }

    if (I != NULL) {
        if (strncmp(I, "TCPS", 4) == 0) {
        I += 4;
        int port = atoi(I);
        inputFD = TCP_SERVER_PORT(port);
    } else if (strncmp(I, "TCPC", 4) == 0) {
        I += 4;
        char *ip = strtok(I, ",");
        char *port_str = strtok(NULL, ",");
        int port = atoi(port_str);
        inputFD = TCP_CLIENT_PORT(ip, port);
    }
    }

    if (B != NULL) {
        if (strncmp(B, "TCPS", 4) == 0) {
            B += 4;
            int port = atoi(B);
            int res_fd = TCP_SERVER_PORT(port);
            inputFD = res_fd;
            outputFD = res_fd; 
        } else if (strncmp(B, "TCPC", 4) == 0) {
    }
    }
    

    // change the input and output to the file descriptor
    dup2(inputFD, STDIN_FILENO);
    dup2(outputFD, STDOUT_FILENO);
    if (E != NULL) {
        run_prog(E);
    }else{
            char buffer[1024]; // Buffer for message transfer
    ssize_t read_bytes, write_bytes;

    // Loop to transfer messages
    while (1) {
        // Read from inputFD
        read_bytes = read(inputFD, buffer, sizeof(buffer));
        if (read_bytes <= 0) {
            // Break the loop if read error or end of file
            break;
        }

        // Write to outputFD
        write_bytes = write(outputFD, buffer, read_bytes);
        if (write_bytes <= 0) {
            // Break the loop if write error
            break;
        }

        // Check for termination condition (e.g., "exit" command)
        // This is a simple example; adjust as needed for your application
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
    }

    }
    

    return 0;
}


// need to clean the buffer in ttt input