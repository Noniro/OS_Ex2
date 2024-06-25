#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char* argv[]) {
    char* E = NULL;

    int opt;

    while ((opt = getopt(argc, argv, "e:")) != -1) {
        switch (opt) {
            case 'e':
                E = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-e string]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    // split argv by space
    if(E != NULL){
    // Count the number of tokens and store them
    int count = 0;
    char* tokens[256];  
    char *token = strtok(E, " ");
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

    return 0;
}
