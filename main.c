#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for random

#define SIZE 5

// Functions
void drawBoard();
void generateNum();

int checkLose();
int checkWin();
void saveScore(char);

// Variables
int board[SIZE][SIZE];
int scores = 0;
int moves = 0;
int timeTaken = 0;
int combo = 0;
char mov;

typedef struct user {
    char username[100];
    int scores;
    int moves;
    int time;
    int combos;
} USER;


int main(void) {
    srand(time(0));
    
    generateNum();
    generateNum();
    drawBoard();

    while(1) {
        if (checkWin()) {
            printf("\n\t\t\tCongratulations, you've got 2048 in %d moves.", moves);
            printf("\n\t\t\tPoints: %d || Time: %d", scores, timeTaken);
            saveScore('y');
            break;
        }

        if (checkLose()) {
            printf("\n\t\t\tGame over.");
            printf("\n\t\tPoints: %d || Time: %d || Combos: %d", scores, timeTaken, combo);
            saveScore('n');
            break;
        }

        scanf(" %c", &mov);
        switch (mov)
        {
        case 'w':
            /* code */
            break;
        
        default:
            break;
        }

        
    }

    return 0;
}

void drawBoard() {
    printf("──────────────────────────\n");

    for (int i=0; i < SIZE; i++) {
        for(int j=0; j < SIZE; j++) {
            if (j == 0) printf("|");
            printf(" %d  |", board[i][j]);
        }
        printf("\n──────────────────────────\n");
    }
}

void generateNum() {
    int horLoc = rand() % SIZE;
    int verLoc = rand() % SIZE;
    //printf("%d %d", horLoc, verLoc);
    int randNumArr[2] = {2,4};

    if (board[horLoc][verLoc] == 0) {
        int num = rand() % 2;
        (num == 0) ? (board[horLoc][verLoc] = 2) : (board[horLoc][verLoc] = 4);
    }


    //printf("%d", board[horLoc][verLoc]);
}


int checkLose() {
    int check = 1;
    // Check whether there are any same numbers in the row
    for (int i=0; i < SIZE; i++) {
        for (int j=0; j < SIZE-1; j++) {
            if (board[i][j+1] == board[i][j]) {
                check = 0;
                return check;
            }
        }
    }

    // Check whether there are any same numbers in the column
    for (int i=0; i < SIZE-1; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == board[i+1][j]) {
                check = 0;
                return check;
            }
        }
    }
    return check;
}

int checkWin() {
    int win = 0;
    for (int i=0; i < SIZE; i++) {
        for (int j=0; j < SIZE; j++) {
            if (board[i][j] == 2048) {
                win = 1;
                return win;
            }
        }
    }
    return win;
}

// Save the score to a TXT file
void saveScore(char success) {
    char username[100];
    FILE *file = fopen("scores.txt", "a");
    
    printf("\n사용자이름 (Player's name): "); 
    scanf("%s", username);

    fprintf("%s\t%s\t%d\t%d\t%d\n", username, success, moves, timeTaken, combo);
    fclose(file);
}

// Sorted ranking: YES (moves -> time taken -> combo -> name) > NO (moves > (same moves -> combos -> time taken -> name))
/* Reading the file, sorting, return 10 first users with their # of moves, time taken and combos
*/




