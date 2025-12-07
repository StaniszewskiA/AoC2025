#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef unsigned int uint;
typedef unsigned long long ull;

#define MAX_ROWS 1024
#define MAX_COLS 1024

static void propagate_beam(
    char** grid, 
    uint rows, 
    uint cols, 
    uint row, 
    uint col, 
    uint* splitCnt
) {
    if (row >= rows || col >= cols) return;

    char currCell = grid[row][col];
    if (currCell == '^') {
        (*splitCnt)++;
        grid[row][col] = 'x';
        if (col > 0) propagate_beam(grid, rows, cols, row + 1, col - 1, splitCnt);
        if (col + 1 < cols) propagate_beam(grid, rows, cols, row + 1, col + 1, splitCnt);
    } else if (currCell == '.' || currCell == 'S') {
        propagate_beam(grid, rows, cols, row + 1, col, splitCnt);
    }
}

static void read_grid(FILE* input, char** grid, uint* rows, uint* cols) {
    char line[MAX_COLS];
    *rows = 0;
    *cols = 0;

    while (fgets(line, sizeof(line), input)) {
        uint len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) len--;

        grid[*rows] = malloc(len + 1);
        strcpy(grid[*rows], line);

        if (len > *cols) *cols = len;
        (*rows)++;
    }
}

void solve_day07a(FILE *input) {
    char* grid[MAX_ROWS];
    uint rows;
    uint cols;
    read_grid(input, grid, &rows, &cols);

    int startCol = -1;
    for (uint col = 0; col < cols; ++col)
        if (grid[0][col] == 'S') { startCol = col; break; }

    if (startCol == -1) return;

    uint res = 0;
    propagate_beam(grid, rows, cols, 1, startCol, &res);

    printf("Res %d\n", res);

    for (uint row = 0; row < rows; ++row) free(grid[row]);
}

static ull memo[MAX_ROWS][MAX_COLS];
static ull propagate_timelines(char** grid, uint rows, uint cols, uint row, uint col) {
    if (row >= rows || col >= cols) return 1;
    if (memo[row][col] != ULLONG_MAX) return memo[row][col];

    char currCell = grid[row][col];
    ull res = 0;

    if (currCell == '^') {
        if (col > 0) res += propagate_timelines(grid, rows, cols, row + 1, col - 1);
        res += propagate_timelines(grid, rows, cols, row + 1, col + 1);
    } else if (currCell == '.' || currCell == 'S') { res = propagate_timelines(grid, rows, cols, row + 1, col); }
    else { res = 0; }

    memo[row][col] = res;
    return res;
}

void solve_day07b(FILE* input) {
    char* grid[MAX_ROWS];
    uint rows;
    uint cols;
    read_grid(input, grid, &rows, &cols);

    int startCol = -1;
    for (uint col = 0; col < cols; ++col) 
        if (grid[0][col] == 'S') { startCol = col; break; }

    if (startCol == -1) return;

    for (uint row = 0; row < rows; ++row)
        for (uint col = 0; col < cols; ++col) memo[row][col] = ULLONG_MAX;

    printf("Res: %llu\n", propagate_timelines(grid, rows, cols, 1, startCol));
    
    for (uint row = 0; row < rows; ++row) free(grid[row]); 
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
        case 1: solve_day07a(input); break;
        case 2: solve_day07b(input); break;
    }

    if (input != stdin) fclose(input);
    return 0;
}
