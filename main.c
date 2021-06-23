#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for random
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>

#define SIZE 5

// Functions
void menu();
void game();
void saveScore(char);
void ranking(); 

// Game Functions
void drawBoard();
void generateNum();
void rotateBoard();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();

void signal_callback_handler(int signum); // register signal handler for when ctrl-c is pressed
void setBufferedInput(bool enable); // Get the key value without pressing Enter
int checkLose();
int checkWin();

// Variables
int board[SIZE][SIZE];
int scores = 0;
int moves = 0;
int timeTaken = 0;
int combo = 0;
char mov;

typedef struct user {
    char username[100];
    char success[3];
    int scores;
    int moves;
    int timeTaken;
    int combos;
} USER;



int main(void) {
    srand(time(0));
    menu();
    return 0;
}

void menu() {
    int opt;
    printf("Choose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
    scanf("%d", &opt);

    while (opt < 5 && opt > 0) {
        if (opt == 1) {
            game();
            printf("Choose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
            scanf("%d", &opt);
        } else if (opt == 2) {
            printf("\nYou just need to move the titles (a: left, s: down, d: right, w: up) and every time you move one, another tile pops up in a random manner anywhere in the box. When two tiles with the same number on them collide with one another as you move the them, they will merge into one tile with the sum of the numbers written on them initially. If you want to quit 2048, just simply press q.\n");
            printf("Choose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
            scanf("%d", &opt);

        } else if (opt == 3) {
            ranking();
            printf("Choose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
            scanf("%d", &opt);
        } else if (opt == 4) {
            return;
        }
        if (opt > 4 || opt < 0) {
            printf("Please choose a different option.\n");
            scanf("%d", &opt);
        }
    }

}

void game() {
    printf("\033[?25l\033[2J");
    signal(SIGINT, signal_callback_handler);

    char mov;

    generateNum();
    generateNum();
    drawBoard();
    setBufferedInput(false);

    printf("w, a, s, d to play the game and q to quit.\n");

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

        scanf("%s", &mov);
        if (mov == 'w') {
            moveUp();
        } if (mov == 'a') {
            moveLeft();
        } if (mov == 's') {
            moveDown();
        } if (mov == 'd') {
            moveRight();
        } if (mov == 'q') {
            char opt;
            printf("QUIT THE GAME? y or n\n");
            scanf("%s", &opt);
            if (opt == 'y') break;
        }
        printf("\033[2J\033[H" ); // Clear the screen
        printf("w, a, s, d to play the game and q to quit.\n");
        
    }
    setBufferedInput(true);
    printf("\033[?25h\033[m");
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
    printf("\n");
}

void generateNum() {
    srand(time(NULL));

    int empty[SIZE*SIZE][2];
    int count = 0;

    for (int i=0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }
        }
    }

    int row, col, val;
    if (rand() % 2) val = 2; else val = 4;
    if (count > 0) {
        int temp = rand() % count;
        row = empty[temp][0];
        col = empty[temp][1];
        board[row][col] = val;
    }
}

// ideas from https://www.geeksforgeeks.org/inplace-rotate-square-matrix-by-90-degrees/
void rotateBoard() {
    int temp;
    for (int i=0; i < SIZE/2; i++) {
        for (int j=i; j < SIZE - i - 1; j++) {
            temp = board[i][j];
            board[i][j] = board[j][SIZE - 1 - i]; // move values from right to top
            board[j][SIZE-1-i] = board[SIZE-1-i][SIZE-1-j]; // move values from bottom to right
            board[SIZE-1-i][SIZE-1-j] = board[SIZE-1-j][i]; // move values from left to bottom
            board[SIZE-1-j][i] = temp; // assign temp to left
        }
    }
}

void moveDown() {

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

void setBufferedInput(bool enable) {
    static bool enabled = true;
    static struct termios old;
    struct termios new;

    if (enable && !enabled) {
        // restore the former settings
        tcsetattr(STDIN_FILENO,TCSANOW,&old);
        // set the new state
        enabled = true;
    } else if (!enable && enabled) {
        // get the terminal settings for standard input
        tcgetattr(STDIN_FILENO,&new);
        // keep the old setting to restore them at the end
        old = new;
        // disable canonical mode (buffered i/o) and local echo
        new.c_lflag &=(~ICANON & ~ECHO);
        // set the new settings immediately
        tcsetattr(STDIN_FILENO,TCSANOW,&new);
        // set the new state
        enabled = false;
    }
}

void signal_callback_handler(int signum) {
    printf("         TERMINATED         \n");
    setBufferedInput(true);
	printf("\033[?25h\033[m");
	exit(signum);
}

// Save the score to a TXT file
void saveScore(char success) {
    char username[100];
    FILE *file = fopen("scores.txt", "a");
    
    printf("\n사용자이름 (Player's name): "); 
    scanf("%s", username);

    fprintf(file, "%s\t%c\t%d\t%d\t%d\n", username, success, moves, timeTaken, combo);
    fclose(file);
}

// Sorted ranking: YES (moves -> time taken -> combo -> name) > NO (moves > (same moves -> combos -> time taken -> name))
/* Reading the file, sorting, return 10 first users with their # of moves, time taken and combos
*/

int compare(const void *s1, const void *s2) {
    USER *e1 = (USER *)s1;
    USER *e2 = (USER *)s2;
    int winornot = e1->success - e2->success;
    if (winornot == 0) {
        if (e1->scores == e2->scores) {
            if (e1->moves == e2-> moves) {
                if (e1->timeTaken == e2->timeTaken) {
                    if (e1->combos == e2->combos) {
                        int nameRanking = strcmp(e1->username, e2->username);
                        return nameRanking;
                    } else return e1->combos - e2->combos;
                } else return e1->timeTaken - e2->timeTaken;

            } else return e1->moves - e2->moves;
        } else return e1->scores - e2->scores;
        
    } else return winornot;
}

void ranking() {
    FILE *fp = fopen("scores.txt", "r");
    USER data;
    int num = 0;
    if (fp == NULL) {
        printf("There are no users. Be the first one today!");
        exit(1);
    }
    
    while (fscanf(fp, "%s %s %d %d %d %d", data.username, data.success, &(data.scores), &(data.moves), &(data.timeTaken), &(data.combos)) == 6) {
        //printf("%s\t%s\t%d\t%d\t%d\t%d\n", data.username, data.success, data.scores, data.moves, data.timeTaken, data.combos);
        num++;
    }

    USER userList[num]; int idx = 0;
    fseek(fp, 0, SEEK_SET); // setting the file pointer to point to the beginning of the file

    char username[100];
    char success[3];
    int scores;
    int moves;
    int timeTaken;
    int combos;

    while (fscanf(fp, "%s %s %d %d %d %d", username, success, &(scores), &(moves), &(timeTaken), &(combos)) == 6) {
        //printf("%s\t%s\t%d\t%d\t%d\t%d\n", username, success, scores, moves, timeTaken, combos);
        strncpy(userList[idx].username, username, 100); 
        userList[idx].username[99] = '\0';
        strncpy(userList[idx].success, success, 3); 
        userList[idx].success[2] = '\0';

        userList[idx].scores = scores;
        userList[idx].moves = moves;
        userList[idx].timeTaken = timeTaken;
        userList[idx].combos = combos;
        idx++;
    }
    
    fclose(fp);

    qsort(userList, num, sizeof(USER), compare);
    printf("\n");
    
    for (int i=0; i < num; i++) {
        printf("Username: %s\t Scores: %d\t Time: %d\n", userList[i].username, userList[i].scores, userList[i].timeTaken);
    }

}






