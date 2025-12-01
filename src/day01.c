

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_helper.h"

#define TASK 1

void solve_day01_a(char **lines, int count) {
    for (int i = 0; i < count; ++i) {
        printf("Task 1: %s\n", lines[i]);
    }
}

void solve_day01_b(char **lines, int count) {
    for (int i = 0; i < count; ++i) {
        printf("Task 2: %s\n", lines[i]);
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    int count = 0;
    char **lines = read_input_file(argv[1], &count);
    if (!lines) return 1;

    switch (TASK) {
        case 1:
            solve_day01_a(lines, count);
            break;
        case 2:
            solve_day01_b(lines, count);
            break;
        default:
            fprintf(stderr, "Unknown TASK: %d\n", TASK);
            for (int i = 0; i < count; ++i) free(lines[i]);
            free(lines);
            return 1;
    }
    for (int i = 0; i < count; ++i) free(lines[i]);
    free(lines);
    return 0;
}
