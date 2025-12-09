#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_POINTS 500
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

typedef unsigned int uint;

typedef struct {
    int id;
    int x;
    int y;
    int compressedX;
    int compressedY;
} CompressablePoint2D;

typedef struct {
    CompressablePoint2D points[MAX_POINTS];
    int numPoints;
} CompressablePointSet;

int cmp_x(const CompressablePoint2D* a, const CompressablePoint2D* b) { return a->x < b->x ? -1 : 1; }
int cmp_y(const CompressablePoint2D* a, const CompressablePoint2D* b) { return a->y < b->y ? -1 : 1; }
int cmp_id(const CompressablePoint2D* a, const CompressablePoint2D* b) { return a->id < b->id ? -1 : 1; }

static inline uint64_t rectangle_area(CompressablePoint2D a, CompressablePoint2D b) {
    return (uint64_t)(abs(b.x - a.x) + 1) * (abs(a.y - b.y) + 1);
}

CompressablePointSet parse_input(FILE* input) {
    CompressablePointSet set = {0};
    while (fscanf(input, "%d,%d", &set.points[set.numPoints].x, &set.points[set.numPoints].y) == 2) {
        set.points[set.numPoints].id = set.numPoints;
        set.numPoints++;
    }
    return set;
}

void compress_coords(CompressablePointSet* set) {
    qsort(set->points, set->numPoints, sizeof(CompressablePoint2D), (int(*)(const void*,const void*))cmp_x);
    int prevX = INT_MIN;
    int compressedX = 0;
    for (int i = 0; i < set->numPoints; i++) {
        if (set->points[i].x != prevX) { compressedX++; prevX = set->points[i].x; }
        set->points[i].compressedX = compressedX;
    }

    qsort(set->points, set->numPoints, sizeof(CompressablePoint2D), (int(*)(const void*,const void*))cmp_y);
    int prevY = INT_MIN;
    int compressedY = 0;
    for (int i = 0; i < set->numPoints; i++) {
        if (set->points[i].y != prevY) { compressedY++; prevY = set->points[i].y; }
        set->points[i].compressedY = compressedY;
    }

    qsort(set->points, set->numPoints, sizeof(CompressablePoint2D), (int(*)(const void*,const void*))cmp_id);
}

void solve_day09a(FILE *input) {
    CompressablePointSet set = parse_input(input);
    compress_coords(&set);

    uint64_t res = 0;
    for (int i = 0; i < set.numPoints; ++i) {
        for (int j = i + 1; j < set.numPoints; ++j) {
            uint64_t area = rectangle_area(set.points[i], set.points[j]);
            res = MAX(res, area);
        }
    }

    printf("Res: %lld\n", res);
}

#define GRID_SIZE 300

void flood_fill_outside(char grid[GRID_SIZE][GRID_SIZE], int row, int col) {
    if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) return;
    if (grid[row][col] != 0) return;

    grid[row][col] = '!';
    flood_fill_outside(grid, row + 1, col);
    flood_fill_outside(grid, row - 1, col);
    flood_fill_outside(grid, row, col + 1);
    flood_fill_outside(grid, row , col - 1);
}

bool is_line_clear(char grid[GRID_SIZE][GRID_SIZE], int x1, int y1, int x2, int y2) {
    if (x1 > x2){ int tmp=x1; x1=x2; x2=tmp; }
    if (y1 > y2){ int tmp=y1; y1=y2; y2=tmp; }

    for (int x = x1; x <= x2; ++x)
        for (int y = y1; y <= y2; ++y) if (grid[y][x] == '!') return false;
    
    return true;
}

bool is_rectangle_valid(char grid[GRID_SIZE][GRID_SIZE], CompressablePoint2D a, CompressablePoint2D b) {
    return is_line_clear(grid, a.compressedX, a.compressedY, b.compressedX, b.compressedY) &&
        is_line_clear(grid, b.compressedX, a.compressedY, b.compressedX, b.compressedY) &&
        is_line_clear(grid, b.compressedX, b.compressedY, a.compressedX, b.compressedY) &&
        is_line_clear(grid, a.compressedX, b.compressedY, a.compressedX, a.compressedY);
}

void solve_day09b(FILE* input) {
    print("TODO");
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
        case 1: solve_day09a(input); break;
        case 2: solve_day09b(input); break;
    }

    if (input != stdin) fclose(input);
    return 0;
}
