#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for random
#include <string.h>

#define SIZE 5

// Functions
void menu();
void game();

// Game Functions
void drawBoard();
void generateNum();

int checkLose();
int checkWin();

// Ranking function
void saveScore(char);
void ranking(); 

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

    
    
    generateNum();
    generateNum();
    drawBoard();

    // while(1) {
    //     if (checkWin()) {
    //         printf("\n\t\t\tCongratulations, you've got 2048 in %d moves.", moves);
    //         printf("\n\t\t\tPoints: %d || Time: %d", scores, timeTaken);
    //         saveScore('y');
    //         break;
    //     }

    //     if (checkLose()) {
    //         printf("\n\t\t\tGame over.");
    //         printf("\n\t\tPoints: %d || Time: %d || Combos: %d", scores, timeTaken, combo);
    //         saveScore('n');
    //         break;
    //     }

    //     scanf(" %c", &mov);
    //     switch (mov)
    //     {
    //     case 'w':
    //         /* code */
    //         break;
        
    //     default:
    //         break;
    //     }

        
    // }

    return 0;
}

void menu() {
    int opt;
    printf("Choose one option to proceed:\n1. Game Start\n2. How to play 2048\n3. Ranking\n4. Exit\n");
    scanf("%d", &opt);

    while (opt < 5 && opt > 0) {
        if (opt == 1) {
            game();
        } else if (opt == 2) {
            printf("\nYou just need to move the titles (a: left, s: down, d: right, w: up) and every time you move one, another tile pops up in a random manner anywhere in the box. When two tiles with the same number on them collide with one another as you move the them, they will merge into one tile with the sum of the numbers written on them initially. If you want to quit 2048, just simply press q.\n");
        } else if (opt == 3) {
            ranking();
        }
        if (opt > 4 || opt < 0) {
            printf("Please choose a different option.\n");
            scanf("%d", &opt);
        }
    }

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
        printf("%s\t%s\t%d\t%d\t%d\t%d\n", username, success, scores, moves, timeTaken, combos);
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
    
    for (int i=0; i < num; i++) {
        printf("%s\t%d\n", userList[i].username, userList[i].scores);
    }

}






