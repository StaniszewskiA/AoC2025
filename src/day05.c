#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "helpers/input_helper.h"

typedef struct {
    long long start;
    long long end;
} Range;

static bool is_in_fresh_range(long long id, Range r) {
    return id >= r.start && id <= r.end;
}

static int read_ranges(
    char** lines, 
    int lineCnt, 
    int startLineIdx, 
    Range** ranges, 
    int* rangeCnt
) {
    int lineIdx = startLineIdx;

    while (lineIdx < lineCnt && strlen(lines[lineIdx]) > 0) {
        long long startIdx;
        long long endIdx;

        if (sscanf(lines[lineIdx], "%lld-%lld", &startIdx, &endIdx) == 2) {
            (*rangeCnt)++;
            *ranges = (Range*)realloc(*ranges, (*rangeCnt) * sizeof(Range));
            if (*ranges == NULL) {
                perror("Failed to reallocate fresh_ranges");
                exit(EXIT_FAILURE);
            }

            (*ranges)[(*rangeCnt) - 1].start = startIdx;
            (*ranges)[(*rangeCnt) - 1].end = endIdx;
        }
        lineIdx++;
    }
    if (lineIdx < lineCnt && strlen(lines[lineIdx]) == 0) lineIdx++;
    return lineIdx;
}

void solve_day05a(char** lines, int lineCnt) {
    Range* freshRanges = NULL;
    int rangeCnt = 0;
    int lineIdx = 0;

    lineIdx = read_ranges(lines, lineCnt, lineIdx, &freshRanges, &rangeCnt);

    long long res = 0;
    for (; lineIdx < lineCnt; lineIdx++) {
        long long availableId;

        if (sscanf(lines[lineIdx], "%lld", &availableId) == 1) {
            bool isFresh = false;

            for (int i = 0; i < rangeCnt; ++i) {
                if (is_in_fresh_range(availableId, freshRanges[i])) {
                    isFresh = true;
                    break;
                }
            }

            if (isFresh) res++;
        }
    }

    printf("Res: %lld\n", res);
    free(freshRanges);
}

int cmp_ranges(const void* a, const void* b) {
    long long startA = ((Range*)a)->start;
    long long startB = ((Range*)b)->start;

    if (startA < startB) return -1;
    if (startA > startB) return 1;
    return 0;
}

#define MAX_LL(a, b) ((a) > (b) ? (a) : (b))

void solve_day05b(char** lines, int lineCnt) {
    Range* freshRanges = NULL;
    int rangeCnt = 0;
    int lineIdx = 0;

    lineIdx = read_ranges(lines, lineCnt, lineIdx, &freshRanges, &rangeCnt);
    if (rangeCnt == 0) {
        printf("Res: 0\n");
        return;
    }

    qsort(freshRanges, rangeCnt, sizeof(Range), cmp_ranges);

    Range* mergedRanges = NULL;
    int mergedCnt = 1;
    mergedRanges = (Range*)malloc(sizeof(Range));
    if (mergedRanges == NULL) {
        perror("Failed to allocate mergedRanges");
        free(freshRanges);
        exit(EXIT_FAILURE);
    }

    mergedRanges[0] = freshRanges[0];

    for (int i = 1; i < rangeCnt; ++i) {
        Range curr = freshRanges[i];
        Range* lastMerged = &mergedRanges[mergedCnt - 1];

        if (curr.start <= lastMerged->end + 1) lastMerged->end = MAX_LL(lastMerged->end, curr.end);
        else {
            mergedCnt++;
            mergedRanges = (Range*)realloc(mergedRanges, mergedCnt * sizeof(Range));
            if (mergedRanges == NULL) {
                perror("Failed to reallocate mergedRanges");
                free(freshRanges);
                exit(EXIT_FAILURE);
            }
            mergedRanges[mergedCnt - 1] = curr;
        }
    }

    long long res = 0;
    for (int i = 0; i < mergedCnt; ++i) res += (mergedRanges[i].end - mergedRanges[i].start + 1);

    printf("Res: %lld\n", res);

    free(freshRanges);
    free(mergedRanges);
}

int main(int argc, char* argv[]) {
    const char* fname = (argc > 1 ? argv[1] : "input");

    int task = 1;
    if (argc >= 3) task = atoi(argv[2]);

    int lineCnt = 0;
    char** lines = read_input_file(fname, &lineCnt);

    switch (task) {
        case 1:
            solve_day05a(lines, lineCnt);
            break;
        
        case 2:
            solve_day05b(lines, lineCnt);
            break;

        default:
            break;
    }

    for (int i = 0; i < lineCnt; ++i) free(lines[i]);
    free(lines);

    return 0;
}
