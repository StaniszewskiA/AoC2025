#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "helpers/input_helper.h"


static bool is_double_number(long long n) {
    char buff[128];
    size_t len;

    int charsWritten = snprintf(buff, sizeof(buff), "%lld", n);
    if (charsWritten <= 0 || (size_t)charsWritten >= sizeof(buff)) return false;

    len = charsWritten;

    if (len % 2 != 0) return false;
    int half = len / 2;

    return strncmp(buff, buff + half, half) == 0;
}


void solve_day02_a(const char* input) {
    long long res = 0;
    char* copy = strdup(input);
    char* token = strtok(copy, ",");
    
    while (token) {
        long long startIdx;
        long long endIdx;
        
        if (sscanf(token, " %lld - %lld", &startIdx, &endIdx) != 2) {
            token = strtok(NULL, ",");
            continue;
        }

        for (long long i = startIdx; i <= endIdx; ++i) if (is_double_number(i)) res += i;

        token = strtok(NULL, ",");
    }

    free(copy);
    printf("Res: %lld\n", res);
}


static bool is_repeated_number(long long n) {
    char buff[128];
    int charsWritten = snprintf(buff, sizeof(buff), "%lld", n);
    if (charsWritten <= 0 || (size_t)charsWritten >= sizeof(buff)) return false;

    int L = charsWritten;

    for (int P = 1; P <= L / 2; ++P) {
        if (L % P != 0) continue;
        int numRepeats = L / P;
        bool isRepeated = true;

        for (int r = 1; r < numRepeats; ++r) {
            if (strncmp(buff, buff + (r * P), P) != 0) {
                isRepeated = false;
                break;
            } 
        }

        if (isRepeated) return true;
    }

    return false;
}


void solve_day02_b(const char* input) {
    long long res = 0;
    char* copy = strdup(input);
    char* token = strtok(copy, ",");
    
    while (token) {
        long long startIdx;
        long long endIdx;
        
        if (sscanf(token, " %lld - %lld", &startIdx, &endIdx) != 2) {
            token = strtok(NULL, ",");
            continue;
        }

        for (long long i = startIdx; i <= endIdx; ++i) if (is_repeated_number(i)) res += i;

        token = strtok(NULL, ",");
    }

    free(copy);
    printf("Res: %lld\n", res);
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        return 1;
    }

    int task = atoi(argv[2]);
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening input file");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *input = malloc(len + 1);
    fread(input, 1, len, f);
    input[len] = '\0';
    fclose(f);

    switch (task) {
        case 1:
            solve_day02_a(input);
            break;
        case 2:
            solve_day02_b(input);
            break;
        default:
            fprintf(stderr, "Unknown task: %d\n", task);
            break;
    }
    free(input);
    return 0;
}
