#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isValidSequence(char *sequence);
void initBoard(char board[3][3]);
void printBoard(char board[3][3]);
int checkWinner(char board[3][3]);
int isValidMove(char board[3][3], int move);
void makeMove(char board[3][3], int move, char player);
void PCmakeMove(char board[3][3], char *sequence, int moveIndex, char player);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <9 digit number>\n", argv[0]);
        
        return 1;
    }

    char *sequence = argv[1];
    if (strlen(sequence) != 9) {
        printf("The sequence must be 9 digits long\n");
        
        return 1;
    }
    if (!isValidSequence(sequence)) {
        printf("The sequence must not contain duplicate digits\n");
        
        return 1;
    }

    char board[3][3];
    initBoard(board);

    makeMove(board, sequence[0] - '0', 'X'); // PC move X.
    printBoard(board);
    printf("\n");

    for (int i = 1; i < 5; i++) {
        int playerMove;
        char input[10];
        printf("Enter your move: ");
        fflush(stdout);
        scanf("%s", input);

        if (strlen(input) != 1 || !isdigit(input[0])) {
            printf("Invalid move: Please enter a digit between 1 and 9\n");
            i--;
            continue;
        }

        playerMove = input[0] - '0';
        if (!isValidMove(board, playerMove)) {
            printf("Invalid move: Cell number %d is taken\n", playerMove);
            i--;
            continue;
        }

        makeMove(board, playerMove, 'O'); // Player move O.
        printBoard(board);
        printf("\n");

        if (checkWinner(board) == 1) {
            printf("You win! >_<\n");
        fflush(stdout);
            return 0;
        }

        PCmakeMove(board, sequence, i, 'X');
        printBoard(board);
        printf("\n");
        

        if (checkWinner(board) == 0) {
            printf("I win! ^_^\n");
        fflush(stdout);
            return 0;
        }
    }

    printf("It's a draw!\n");
        fflush(stdout);
    return 0;
}

int isValidSequence(char *sequence) {
    int seen[10] = {0};
    for (int i = 0; i < 9; i++) {
        int digit = sequence[i] - '0';
        if (digit < 1 || digit > 9 || seen[digit]) {
            return 0;
        }
        seen[digit] = 1;
    }
    return 1;
}

void initBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        if (i > 0) {
            printf(" --+---+--\n");
            
        }
        for (int j = 0; j < 3; j++) {
            if (j > 0) {
                printf("|");
            
            }
            printf(" %c ", board[i][j]);
            
        }
        printf("\n");
            
        
    }
}

int checkWinner(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        // Check rows and columns for 'X' or 'O' win
        if ((board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') ||
            (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')) {
            return board[i][i] == 'X' ? 0 : 1;
        }
    }
    // Check diagonals for 'X' or 'O' win
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')) {
        return board[1][1] == 'X' ? 0 : 1;
    }
    // If no win condition is met, return -1
    return -1;
}

int isValidMove(char board[3][3], int move) {
    if (move < 1 || move > 9) {
        return 0;
    }
    int i = (move - 1) / 3;
    int j = (move - 1) % 3;
    return board[i][j] == ' ';
}

void makeMove(char board[3][3], int move, char player) {
    int i = (move - 1) / 3;
    int j = (move - 1) % 3;
    board[i][j] = player;
}

void PCmakeMove(char board[3][3], char *sequence, int moveIndex, char player) {
    for (int i = moveIndex; i < 9; i++) {
        int move = sequence[i] - '0';
        if (isValidMove(board, move)) {
            makeMove(board, move, player);
            return;
        }
    }
}
