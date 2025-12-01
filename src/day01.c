

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_helper.h"

#define MOD 100

static int get_new_pos(int currPos, int steps) {
    return (currPos + steps % MOD + MOD) % MOD;
}

void solve_day01_a(char **instructions, int count) {
    int currIdx = 50;
    int res = 0;

    for (int i = 0; i < count; ++i) {
        char* line = instructions[i];
        int sign = (line[0] == 'L') ? -1 : 1;
        int dist = 0;
        dist = atoi(line + 1);
        int step = sign * dist;

        currIdx = get_new_pos(currIdx, step);

        if (currIdx == 0) res += 1;
    }

    printf("res: %i\n", res);
}

void solve_day01_b(char **instructions, int count) {
    int currIdx = 50;
    int res = 0;

    for (int i = 0; i < count; ++i) {
        char* line = instructions[i];
        int sign = (line[0] == 'L') ? -1 : 1;
        int dist = 0;
        dist = atoi(line + 1);
        int start = currIdx;
        int steps = dist;

        for (int step = 1; step <= steps; ++step) {
            int pos = get_new_pos(start, sign * step);
            if (pos == 0) res ++;
        }

        currIdx = (currIdx + sign * dist + MOD) % MOD;
    }    

    printf("res: %i\n", res);
}


int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    int task = 1;
    if (argc >= 3) task = atoi(argv[2]);

    int count = 0;
    char **instructions = read_input_file(argv[1], &count);
    if (!instructions) return 1;

    switch (task) {
        case 1:
            solve_day01_a(instructions, count);
            break;
        case 2:
            solve_day01_b(instructions, count);
            break;
        default:
            fprintf(stderr, "Unknown TASK: %d\n", task);
            for (int i = 0; i < count; ++i) free(instructions[i]);
            free(instructions);
            return 1;
    }
    for (int i = 0; i < count; ++i) free(instructions[i]);
    free(instructions);
    return 0;
}
