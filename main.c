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
void reset();

void signal_callback_handler(int signum); // register signal handler for when ctrl-c is pressed
void setBufferedInput(bool enable); // Get the key value without pressing Enter
int record(int); // saving the record
int lose();
int win();

// Variables
int board[SIZE][SIZE];
int scores = 0;
int moves = 0;
int combos = 0;
char mov;

int start_min;
int start_sec;
int end_min;
int end_sec;
int time_left;

typedef struct user {
    char username[100];
    char success[3];
    int scores;
    int moves;
    int combos;
} USER;

int main(void) {
    printf("\t\t\t\t2048\t\t\t\t\n");
    printf("\t\t\tBy Minh Luong-Do Ngoc @ SNU\n");
    srand(time(0));
    menu();
    return 0;
}

void menu() {
    int opt;
    printf("Choose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
    scanf("%d", &opt);

    while (1) {
        if (opt == 1) {
            game();
            printf("\nChoose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
            printf("\n");
            scanf("%d", &opt);
        } else if (opt == 2) {
            printf("\nYou just need to move the titles (a: left, s: down, d: right, w: up) and every time you move one, another tile pops up in a random manner anywhere in the box. When two tiles with the same number on them collide with one another as you move the them, they will merge into one tile with the sum of the numbers written on them initially. If you want to quit 2048, just simply press q.\n");
            printf("Choose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
            scanf("%d", &opt);

        } else if (opt == 3) {
            ranking();
            printf("\nChoose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
            scanf("%d", &opt);
        } else if (opt == 4) {
            return;
        }
        if (opt > 4 || opt < 1) {
            printf("Please choose a different option.\n");
            scanf("%d", &opt);
        }
    }

}

void game() {
    time_t breakTime;
    time_t startTime = time(NULL);
    time_t seconds = 300; // 5 minutes 
    breakTime = startTime + seconds;

    printf("\033[?25l\033[2J");
    signal(SIGINT, signal_callback_handler);

    char mov;
    printf("Scores: %d %*s%d\n", scores, 20, "Moves: ", moves);
    printf("Time left: %ld seconds\n\n", breakTime - startTime);

    generateNum();
    generateNum();
    drawBoard();
    setBufferedInput(false);

    printf("w, a, s, d to play the game and q to quit.\n");
    
    int check = 0; 
    int in_loop = 0;

    while(time(NULL) < breakTime) {
        //startTime = time(NULL);
        check = record(!in_loop);
        if (check) break;

        mov = getchar();
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
            setBufferedInput(true);
            printf("QUIT THE GAME? y or n\n");
            scanf("%s", &opt);
            if (opt == 'y') {
                break;
            }
            setBufferedInput(false);
        }
        //clock_t betweenTime = clock();

        printf("\033[2J\033[H" ); // Clear the screen
        printf("Scores: %d %*s%d\n", scores, 20, "Moves: ", moves);
        printf("Time left: %ld seconds\n", breakTime - time(NULL)); // Update time after updating the board
        printf("\n");
        drawBoard();
        printf("w, a, s, d to play the game and q to quit.\n");
        
    }
    if (!check) {
        check = record(in_loop);
    }
    setBufferedInput(true);
    reset();
    printf("\033[?25h\033[m");
}

int record(int in_loop) {
    int rec = 0;
    if (win()) {
            printf("\nCongratulations, you've got 2048 in %d moves.", moves);
            printf("\nPoints: %d || Combos: %d", scores, combos);
            setBufferedInput(true);
            saveScore('y');
            rec = 1;
            return rec;
    }

    else if (lose()) {
        printf("\n\tGame over.");
        printf("\nPoints: %d || Combos: %d", scores, combos);
        setBufferedInput(true);
        saveScore('n');
        rec = 1;
        return rec;
    } else if (!in_loop) {
        printf("\n\tGame over.");
        printf("\nPoints: %d || Combos: %d", scores, combos);
        setBufferedInput(true);
        saveScore('n');
        rec = 1;
        return rec;
    }
    return rec;
}

void drawBoard() {
    printf("───────────────────────────────\n");

    for (int i=0; i < SIZE; i++) {
        for(int j=0; j < SIZE; j++) {
            if (j == 0) printf("|");
            printf("%*d|", 5, board[i][j]);
        }
        printf("\n───────────────────────────────\n");
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

// moving to the right
int moveBoard() {
    int done = 0;
    int nextPos, currPos;
    for (int i=0; i < SIZE; i++) {
        for (int j = SIZE-1; j > -1; j--) { 
            if (board[i][j] != 0) { 
                nextPos = j+1;
                currPos = nextPos-1;
                
                // merge
                while (nextPos < SIZE && board[i][currPos] == board[i][nextPos]) {
                    board[i][nextPos] *= 2;
                    board[i][currPos] = 0;
                    scores += board[i][nextPos];
                    combos++;
                    done = 1;
                }

                // move
                while (nextPos < SIZE && board[i][nextPos] == 0) {
                    board[i][nextPos] = board[i][currPos];
                    board[i][currPos] = 0;
                    done = 1;
                    nextPos++;
                    currPos++;
                }
            }
        }
    }

    moves++;
        
    return done;
}

void moveUp() {
    rotateBoard();
    rotateBoard();
    rotateBoard();
    if(moveBoard()){
        generateNum();
        // generateNum();
    }  
    rotateBoard();
}

void moveDown() {
    rotateBoard();
    if(moveBoard()){
        generateNum();
        //generateNum();
    }
    rotateBoard();
    rotateBoard();
    rotateBoard();
}

void moveLeft() {
    rotateBoard();
    rotateBoard();
    if (moveBoard()) {
        generateNum();
    }
    rotateBoard();
    rotateBoard();
}

void moveRight() {
    if (moveBoard()){
        generateNum();
        //generateNum();
    }
}

void reset() {
    for (int i=0; i < SIZE; i++) {
        for (int j=0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    scores = 0;
    moves = 0;
    combos = 0;
}

int lose() {
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

int win() {
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

    fprintf(file, "%s\t%c\t%d\t%d\t%d\n", username, success, scores, moves, combos);
    fclose(file);
}

// Sorted ranking: YES (moves -> time taken -> combo -> name) > NO (moves > (same moves -> combos -> time taken -> name))
/* Reading the file, sorting, return users with their # of moves, time taken and combos
*/

int compare(const void *s1, const void *s2) {
    USER *e1 = (USER *)s1;
    USER *e2 = (USER *)s2;
    int winornot = strcmp(e1->success, e2->success);
    //printf("%d\n", winornot);
    if (winornot == 0) {
        if (e1->scores == e2->scores) {
            if (e1->moves == e2-> moves) {
                int nameRanking = strcmp(e1->username, e2->username);
                return nameRanking;
            } return (e1->moves - e2->moves);
        } return -(e1->scores - e2->scores);  
    } 
    return -winornot;
}

void ranking() {
    FILE *fp = fopen("scores.txt", "r");
    USER data;
    int num = 0;
    if (fp == NULL) {
        printf("There are no users. Be the first one today!\n\n");
        fclose(fp);
        menu();
    }

    while (fscanf(fp, "%s %s %d %d %d", data.username, data.success, &(data.scores), &(data.moves), &(data.combos)) == 5) {
            //printf("%s\t%s\t%d\t%d\t%d\t%d\n", data.username, data.success, data.scores, data.moves, data.timeTaken, data.combos);
        num++;
    }
    //printf("%d", num);
    USER userList[num]; int idx = 0;
    fseek(fp, 0, SEEK_SET); // setting the file pointer to point to the beginning of the file

    char username[100];
    char success[3];
    int scores;
    int moves;
    int combos;

    while (fscanf(fp, "%s %s %d %d %d", username, success, &(scores), &(moves), &(combos)) == 5) {
            //printf("%s\t%s\t%d\t%d\t%d\t%d\n", username, success, scores, moves, timeTaken, combos);
        strncpy(userList[idx].username, username, 100); 
        userList[idx].username[99] = '\0';
        strncpy(userList[idx].success, success, 3); 
        userList[idx].success[2] = '\0';

        userList[idx].scores = scores;
        userList[idx].moves = moves;
        userList[idx].combos = combos;
        idx++;
    }
    
    
    fclose(fp);
        
    qsort(userList, num, sizeof(USER), compare);
    printf("\n");
        
    for (int i=0; i < num; i++) {
        printf("Username: %-7s\tScores: %5d\tMoves: %d\n", userList[i].username, userList[i].scores, userList[i].moves);
    }
    
}
    

    








