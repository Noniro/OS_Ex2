#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

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
    int optval = 1;
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

int UDP_SERVER_PORT(int port){
    int sockfd;

    // open a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("UDP Socket created\n");

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

    //recieve from client dummy message
    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len) < 0) {
        perror("recvfrom");
        exit(1);
    }    

    // connect to the client to save the address
    if (connect(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("connect");
        exit(1);
    }
    return sockfd;
}

int UDP_CLIENT_PORT(char *ip, int port){
    int sockfd;

    // open a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("UDP Socket created\n");

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

int UDS_SERVER_DATAGRAM(char *path) {
    int sockfd;

    // open a Unix domain socket
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("Unix Datagram Socket created\n");

    // bind the socket to the address
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, path, sizeof(server_addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("Socket bound to path: %s\n", path);

    //receive from client dummy message
    char buffer[1024];
    struct sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len) < 0) {
        perror("recvfrom");
        exit(1);
    }

    return sockfd;
}

int UDS_CLIENT_DATAGRAM(char *path) {
    int sockfd;

    // open a Unix domain socket
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("Unix Datagram Socket created\n");

    // connect to the server
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, path, sizeof(server_addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server path: %s\n", path);

    return sockfd;
}

int UDS_SERVER_STREAM(char *path) {
    int sockfd;

    // open a Unix domain socket
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("Unix Stream Socket created\n");

    // bind the socket to the address
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, path);
    printf("Path: %s\n", server_addr.sun_path);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("Socket bound to path: %s\n", path);

    // listen for incoming connections
    if (listen(sockfd, 1) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Listening for incoming connections\n");

    // accept an incoming connection
    struct sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd;

    if ((client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
        perror("accept");
        exit(1);
    }

    printf("Accepted incoming connection\n");

    return client_sockfd;
}

int UDS_CLIENT_STREAM(char *path) {
    int sockfd;

    // open a Unix domain socket
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    printf("Unix Stream Socket created\n");

    // connect to the server
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, path, sizeof(server_addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server path: %s\n", path);

    return sockfd;
}

void alarm_handler(int sig) {
    fprintf(stderr, "Timeout\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    char* E = NULL;
    char* O = NULL;
    char* I = NULL;
    char* B = NULL;
    int timeout = 0;

    int opt;

    int inputFD = STDIN_FILENO;
    int outputFD = STDOUT_FILENO;
    // Parse the command line arguments using getopt 
    while ((opt = getopt(argc, argv, "e:i:b:o:t:")) != -1) {
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
            case 't':
                timeout = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-e string]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (timeout > 0) {
        signal(SIGALRM, alarm_handler);
        alarm(timeout);
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
        } else if (strncmp(O, "UDPS", 4) == 0) {
            O += 4;
            int port = atoi(O);
            outputFD = UDP_SERVER_PORT(port);
        } else if (strncmp(O, "UDPC", 4) == 0) {
            O += 4;
            char *ip = strtok(O, ",");
            char *port_str = strtok(NULL, ",");
            int port = atoi(port_str);
            outputFD = UDP_CLIENT_PORT(ip, port);
        } else if (strncmp(O, "UDSCD", 5) == 0) {
            O += 5;
            outputFD = UDS_CLIENT_DATAGRAM(O);
        } else if (strncmp(O, "UDSCS", 5) == 0) {
            O += 5;
            outputFD = UDS_CLIENT_STREAM(O);
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
        } else if (strncmp(I, "UDPS", 4) == 0) {
            I += 4;
            int port = atoi(I);
            inputFD = UDP_SERVER_PORT(port);
        } else if (strncmp(I, "UDPC", 4) == 0) {
            I += 4;
            char *ip = strtok(I, ",");
            char *port_str = strtok(NULL, ",");
            int port = atoi(port_str);
            inputFD = UDP_CLIENT_PORT(ip, port);
        } else if (strncmp(I, "UDSSD", 5) == 0) {
            I += 5;
            inputFD = UDS_SERVER_DATAGRAM(I);
        } else if (strncmp(I, "UDSSS", 5) == 0) {
            I += 5;
            inputFD = UDS_SERVER_STREAM(I);
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
            B += 4;
            char *ip = strtok(B, ",");
            char *port_str = strtok(NULL, ",");
            int port = atoi(port_str);
            int res_fd = TCP_CLIENT_PORT(ip, port);
            inputFD = res_fd;
            outputFD = res_fd;
        } else if (strncmp(B, "UDPS", 4) == 0) {
            B += 4;
            int port = atoi(B);
            int res_fd = UDP_SERVER_PORT(port);
            inputFD = res_fd;
            outputFD = res_fd;
        } else if (strncmp(B, "UDPC", 4) == 0) {
            B += 4;
            char *ip = strtok(B, ",");
            char *port_str = strtok(NULL, ",");
            int port = atoi(port_str);
            int res_fd = UDP_CLIENT_PORT(ip, port);
            inputFD = res_fd;
            outputFD = res_fd;
        }else if(strncmp(B, "UDSSD", 5) == 0){
            B += 5;
            int res_fd = UDS_SERVER_DATAGRAM(B);
            inputFD = res_fd;
            outputFD = res_fd;
        }else if(strncmp(B, "UDSSS", 5) == 0){
            B += 5;
            int res_fd = UDS_SERVER_STREAM(B);
            inputFD = res_fd;
            outputFD = res_fd;
        }
    }

    // change the input and output to the file descriptor
    dup2(inputFD, STDIN_FILENO);
    dup2(outputFD, STDOUT_FILENO);
    
    if (E != NULL) {
        printf("Running execvp\n");
        run_prog(E);
    } else {
        printf("Running chat\n");
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
