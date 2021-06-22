#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user {
    char username[100];
    char success[3];
    int scores;
    int moves;
    int timeTaken;
    int combos;
} USER;

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
    //fprintf(fp, "abcxuz\n");
    
    while (fscanf(fp, "%s %s %d %d %d %d", data.username, data.success, &(data.scores), &(data.moves), &(data.timeTaken), &(data.combos)) == 6) {
        //printf("%s\t%s\t%d\t%d\t%d\t%d\n", data.username, data.success, data.scores, data.moves, data.timeTaken, data.combos);
        num++;
    }

    USER userList[num]; int idx = 0;
    fseek(fp, 0, SEEK_SET); // setting the file pointer to point to the beginning of the file

    char username[100];
    char success];
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
    
    // while (fscanf(fp, "%s %s %d %d %d %d", data.username, data.success, &(data.scores), &(data.moves), &(data.timeTaken), &(data.combos)) == 6) {
    //     printf("%s\t%s\t%d\t%d\t%d\t%d\n", data.username, data.success, data.scores, data.moves, data.timeTaken, data.combos);
    // }
    
    fclose(fp);

    qsort(userList, num, sizeof(USER), compare);
    
    for (int i=0; i < num; i++) {
        printf("%d\t%s\t%d\n", i, userList[i].username, userList[i].scores);
    }

}

int main() {
    ranking();
    return 0;
}
