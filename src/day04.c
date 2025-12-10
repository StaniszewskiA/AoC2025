#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>

#define ROLL '@'
#define SPACE '.'
#define BOXED 4
#define N 150
#define R_SIZE (N + 2)
#define C_SIZE (N + 3)

#define GRID_ACCESS(grid, row, col) ((char*)(grid))[(row) * C_SIZE + (col)]

static int forklift(char* gridPtr, const bool remove) {
    int res = 0;

    for (int i = 1; i < N + 1; ++i) {
        for (int j = 1; j < N + 1; ++j) {
            if (GRID_ACCESS(gridPtr, i, j) != ROLL) continue;
            const int y0 = i - 1;
            const int y1 = i + 2;
            const int x0 = j - 1;
            const int x1 = j + 2;
            int sum = 0;

            for (int y = y0; y < y1; ++y) 
                for (int x = x0; x < x1; ++x) sum += GRID_ACCESS(gridPtr, y, x) == ROLL;

            if (sum < BOXED + 1) {
                ++res;
                if (remove) GRID_ACCESS(gridPtr, i, j) = SPACE;
            }
        }
    }

    return res;
}

void solve_day04_a(char* grid) { printf("Res: %d\n", forklift(grid, false)); }

void solve_day04_b(char* grid) {
    int res = 0;
    int currRemoved;

    do {
        currRemoved = forklift(grid, true);
        res += currRemoved;
    } while (currRemoved != 0);

    printf("Res: %d\n", res);
}


int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    int task = 1;
    if (argc >= 3) task = atoi(argv[2]);

    const char* fname = argv[1];

    char grid[R_SIZE][C_SIZE] = {0};

    FILE* f = fopen(fname, "r");
    if (!f) { fprintf(stderr, "File not found: %s\n", fname); return 1; }
    for (int i = 1; i <= N && fgets(&grid[i][1], N + 2, f); ++i);
    fclose(f);

    char gridCopy[R_SIZE][C_SIZE];
    memcpy(gridCopy, grid, sizeof(grid));

    switch (task) {
        case 1:
            solve_day04_a((char*)grid);
            break;

        case 2:
            solve_day04_b((char*)gridCopy);
            break;

        default:
            return 1;
    }

    return 0;
}
