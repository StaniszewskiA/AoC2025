#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROWS 1024
#define MAX_COLS 4096

void solve_day06a(FILE *input) {
    char line[MAX_COLS];
    unsigned long numsPerRow[32][1024] = {0};
    unsigned long res = 0;
    unsigned int row = 0;

    while (fgets(line, sizeof(line), input)) {
        unsigned int col = 0;
        char* tok = strtok(line, " \n");
        while (tok) {
            if (strchr(tok, '+')) {
                unsigned long subRes = 0;
                for (unsigned int i = 0; i < row; ++i) subRes += numsPerRow[i][col];
                res += subRes;
            } else if (strchr(tok, '*')) {
                unsigned long long subRes = 1;
                for (unsigned int i = 0; i < row; ++i) subRes *= numsPerRow[i][col];
                res += subRes;
            } else numsPerRow[row][col] = atol(tok);
            tok = strtok(NULL, " \n");
            col++;
        }
        row++;
    }

    printf("Res: %lu\n", res);
}

void solve_day06b(FILE *input) {
    char line[MAX_COLS];
    char* rows[MAX_ROWS];
    unsigned int rowCnt = 0;

    while (fgets(line, sizeof(line), input)) {
        unsigned int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) line[--len] = 0;
        rows[rowCnt] = malloc(len + 1);
        strcpy(rows[rowCnt], line);
        rowCnt++;
    }

    unsigned long res = 0;
    unsigned int width = strlen(rows[0]);
    unsigned int col = 0;

    while (col < width) {
        int isEmpty = 1;
        for (unsigned int row = 0; row < rowCnt; row++)
            if (rows[row][col] != ' ') { isEmpty=0; break; }
        if (isEmpty) { col++; continue; }

        unsigned int start = col;
        while (col < width) {
            int all_space = 1;
            for (unsigned int row = 0; row < rowCnt; row++)
                if (rows[row][col] != ' ') { all_space=0; break; }
            if (all_space) break;
            col++;
        }
        unsigned int end = col-1;

        unsigned long nums[1024];
        unsigned int cnt = 0;

        for (unsigned int col = start; col <= end; col++) {
            char buf[32]; 
            unsigned int bi = 0;
            for (unsigned int row = 0; row < rowCnt-1; row++)
                if (isdigit(rows[row][col])) buf[bi++] = rows[row][col];
            if (bi==0) continue;
            buf[bi]=0;
            nums[cnt++] = atol(buf);
        }

        char op = '+';
        for (unsigned int col = start; col <= end; ++col)
            if (rows[rowCnt - 1][col] == '+' || rows[rowCnt - 1][col] == '*')
                op = rows[rowCnt - 1][col];

        unsigned long subRes = (op == '+') ? 0 : 1;
        for (unsigned int i = 0; i < cnt; ++i) subRes = (op == '+') ? subRes + nums[i] : subRes*nums[i];
        res += subRes;
    }

    printf("Res: %lu\n", res);

    for (unsigned int row = 0; row < rowCnt; ++row) free(rows[row]);
}

int main(int argc, char* argv[]) {
    int task = 1;
    if (argc >= 3) task = atoi(argv[2]);

    FILE *input = stdin;
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Failed to open input file: %s\n", argv[1]);
            return 1;
        }
    }

    switch (task) {
        case 1: solve_day06a(input); break;
        case 2: solve_day06b(input); break;
    }

    if (input != stdin) fclose(input);
    return 0;
}
