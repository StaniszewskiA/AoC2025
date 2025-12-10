#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LEN 4096
#define MAX_M 64
#define MAX_N 64
#define MAX_BUF_SIZE 256
#define BASIS_SIZE 64

typedef unsigned long long ull;


static ull parse_diagram(const char* s, int* nLightsOut) {
    ull mask = 0;
    int bit = 0;
    for (const char* p = s; *p; ++p) {
        if (*p == '.' || *p == '#') {
            if (*p == '#') mask |= (1ULL << bit);
            bit++;
        }
    }
    *nLightsOut = bit;
    return mask;
}

static ull parse_button(const char* s) {
    ull mask = 0;
    int currNum = 0;
    bool readingNum = false;

    for (const char* p = s; *p; ++p) {
        if (isdigit(*p)) {
            currNum = currNum * 10 + (*p - '0');
            readingNum = true;
        } else if (readingNum) {
            mask |= (1ULL << currNum);
            currNum = 0;
            readingNum = false;
        }
    }
    if (readingNum) mask |= (1ULL << currNum);
    return mask;
}

static bool parse_machine_line(
    const char* line,
    ull* targetOut,
    int* nLightsOut,
    ull* buttonsOut,
    int* nButtonsOut
) {
    *nButtonsOut = 0;

    const char* lb = strchr(line, '[');
    const char* rb = strchr(line, ']');
    if (!lb || !rb) return false;

    char diag[MAX_BUF_SIZE];
    int len = rb - lb + 1;
    memcpy(diag, lb, len);
    diag[len] = '\0';

    *targetOut = parse_diagram(diag, nLightsOut);

    const char* p = rb;
    while ((p = strchr(p, '(')) != NULL) {
        const char* q = strchr(p, ')');
        if (!q) break;

        char buf[MAX_BUF_SIZE];
        int bufLen = q - p + 1;
        memcpy(buf, p, bufLen);
        buf[bufLen] = '\0';

        buttonsOut[*nButtonsOut] = parse_button(buf);
        (*nButtonsOut)++;
        p = q + 1;
    }

    return true;
}

static void build_system_matrix(ull rows[], ull buttons[], int N, int M) {
    for (int i = 0; i < N; ++i) {
        rows[i] = 0;
        for (int button = 0; button < M; ++button)
            if ((buttons[button] >> i) & 1ULL) rows[i] |= (1ULL << button);
    }
}

static inline int popcount64(ull x) {
    return __builtin_popcountll(x);
}

static int gauss_solve_min_weight(ull rows[], ull target, int N, int M) {
    ull A[MAX_N];
    int b[MAX_N];

    for (int i = 0; i < N; i++) {
        A[i] = rows[i];
        b[i] = (target >> i) & 1;
    }

    int pivotCol[MAX_N];
    memset(pivotCol, -1, sizeof(pivotCol));
    int row = 0;

    // Forward elimination
    for (int c = 0; c < M && row < N; c++) {
        int selectedCol = -1;
        for (int i = row; i < N; i++)
            if ((A[i] >> c) & 1ULL) { selectedCol = i; break; }
        if (selectedCol == -1) continue;

        ull tmpA = A[row]; A[row] = A[selectedCol]; A[selectedCol] = tmpA;
        int tmpB = b[row]; b[row] = b[selectedCol]; b[selectedCol] = tmpB;

        pivotCol[row] = c;

        for (int i = row+1; i < N; i++)
            if ((A[i] >> c) & 1ULL) { A[i] ^= A[row]; b[i] ^= b[row]; }

        row++;
    }

    int rank = row;
    for (int i = rank; i < N; i++)
        if (A[i] == 0ULL && b[i] == 1) return -1;

    ull x_p = 0;
    for (int i = rank - 1; i >= 0; i--) {
        int pc = pivotCol[i];
        int sum = b[i];
        for (int c = pc + 1; c < M; c++)
            if ((A[i] >> c) & 1ULL) sum ^= (int)((x_p >> c) & 1ULL);
        if (sum & 1) x_p |= (1ULL << pc);
    }

    int isPivot[MAX_M] = {0};
    for (int i = 0; i < rank; i++) isPivot[pivotCol[i]] = 1;

    int freeVars[MAX_M], freeVarsCnt = 0;
    for (int c = 0; c < M; c++)
        if (!isPivot[c]) freeVars[freeVarsCnt++] = c;

    ull basis[BASIS_SIZE];
    for (int k = 0; k < freeVarsCnt; k++) {
        int fvVec = freeVars[k];
        ull vec = (1ULL << fvVec);
        for (int i = rank - 1; i >= 0; i--) {
            int pc = pivotCol[i];
            int sum = (vec >> pc) & 1ULL;
            for (int c = pc + 1; c < M; c++)
                if ((A[i] >> c) & 1ULL) sum ^= (int)((vec >> c) & 1ULL);
            if (sum & 1) vec |= (1ULL << pc);
            else vec &= ~(1ULL << pc);
        }
        basis[k] = vec;
    }

    int res = -1;
    ull limit = 1ULL << freeVarsCnt;
    for (ull mask = 0; mask < limit; mask++) {
        ull curr = x_p;
        for (int i = 0; i < freeVarsCnt; i++)
            if ((mask >> i) & 1ULL) curr ^= basis[i];
        int w = popcount64(curr);
        if (res < 0 || w < res) res = w;
    }

    return res;
}

static int min_presses_for_machine(const char* line) {
    ull target;
    int N, M;
    ull buttons[MAX_M];

    if (!parse_machine_line(line, &target, &N, buttons, &M)) return 0;

    ull rows[MAX_N];
    build_system_matrix(rows, buttons, N, M);
    return gauss_solve_min_weight(rows, target, N, M);
}

void solve_day10a(FILE *input) {
    char line[MAX_LEN];
    long res = 0;
    while (fgets(line, sizeof(line), input)) {
        int cost = min_presses_for_machine(line);
        res += cost;
    }
    printf("Res: %ld\n", res);
}

void solve_day10b(FILE* input) {
    printf("TODO: Currently implemented in Julia!\n");
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
        case 1: solve_day10a(input); break;
        case 2: solve_day10b(input); break;
    }

    if (input != stdin) fclose(input);
    return 0;
}
