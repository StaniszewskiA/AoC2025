#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_SHAPES 6
#define SHAPE_SIZE 3

typedef char Shape[SHAPE_SIZE][SHAPE_SIZE];
typedef long long ll;

static bool read_shape(FILE* input, Shape shape, int* idx) {
    if (fscanf(input, "%d:\n", idx) != 1) return false;
    for (int y = 0; y < SHAPE_SIZE; ++y) {
        for (int x = 0; x < SHAPE_SIZE; ++x) shape[y][x] = fgetc(input);
        fgetc(input);
    }
    fgetc(input);
    return true;
}

static int compute_shape_area(const Shape shape) {
    int area = 0;
    for (int y = 0; y < SHAPE_SIZE; ++y)
        for (int x = 0; x < SHAPE_SIZE; ++x)
            if (shape[y][x] == '#') area++;

    return area;
}

static bool read_region(FILE* input, int* w, int* h, int cnts[NUM_SHAPES]) {
    int region = fscanf(input,
        "%dx%d: %d %d %d %d %d %d\n",
        w, h,
        &cnts[0], &cnts[1], &cnts[2],
        &cnts[3], &cnts[4], &cnts[5]);
    return (region == 8);
}

static bool region_fits(
    int w,
    int h,
    const char sizes[NUM_SHAPES],
    const int cnts[NUM_SHAPES]
) {
    ll regionArea = (ll)w * h;
    ll shapeArea = 0;

    for (int i = 0; i < NUM_SHAPES; ++i) shapeArea += (ll)sizes[i] * cnts[i];

    return shapeArea < regionArea;
}

void solve_day12a(FILE *input) {
    Shape shapes[NUM_SHAPES];
    char sizes[NUM_SHAPES];

    for (int read = 0; read < NUM_SHAPES; ) {
        int idx;
        if (!read_shape(input, shapes[idx = read], &idx)) {
            fprintf(stderr, "Expected shape index\n");
            return;
        }
        if (idx != read) {
            fprintf(stderr, "Shapes out of order\n");
            return;
        }
        read++;
    }

    for (int i = 0; i < NUM_SHAPES; ++i) sizes[i] = compute_shape_area(shapes[i]);

    int fits = 0;
    int w;
    int h;
    int cnts[NUM_SHAPES];

    while (read_region(input, &w, &h, cnts))
        if (region_fits(w, h, sizes, cnts)) fits++;

    printf("Res %d\n", fits);
}

void solve_day12b(FILE* input) {
    printf("Nothing to be done here.\n");
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
        case 1: solve_day12a(input); break;
        case 2: solve_day12b(input); break;
    }

    if (input != stdin) fclose(input);
    return 0;
}
