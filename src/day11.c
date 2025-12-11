#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_N 2048
#define MAX_NAME_LEN 4
#define MAX_EDGES 64
#define MAX_LINE_LEN 256

typedef long long ll;

typedef struct {
    int nameCnt;
    char names[MAX_N][MAX_NAME_LEN];
    int edges[MAX_N][MAX_EDGES];
    int edgeCnt[MAX_N];
} Graph;

static void graph_init(Graph* g) {
    g->nameCnt = 0;
    memset(g->edgeCnt, 0, sizeof(g->edgeCnt));
}

static int get_id(Graph* g, const char* s) {
    for (int i = 0; i < g->nameCnt; ++i)
        if(strcmp(g->names[i], s) == 0) return i;

    strcpy(g->names[g->nameCnt], s);
    return g->nameCnt++;
}

static void read_graph(Graph* g, FILE* input) {
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), input)) {
        char* p = strchr(line, ':');
        if (!p) continue;
        *p = '\0';

        char left[MAX_NAME_LEN];
        strncpy(left, line, MAX_NAME_LEN);

        int u = get_id(g, left);
        p++;
        char* tok = strtok(p, " \t\n");
        while(tok) {
            int v = get_id(g, tok);
            g->edges[u][g->edgeCnt[u]++] = v;
            tok = strtok(NULL, " \t\n");
        }
    }
}

typedef struct {
    ll memo[MAX_N];
    bool used[MAX_N];
    Graph* g;
    int target;
} DFS_A;

static ll dfs_a(DFS_A* ctx, int u) {
    if (u == ctx->target) return 1;
    if (ctx->used[u]) return ctx->memo[u];

    ctx->used[u] = true;
    ll res = 0;

    for (int i = 0; i < ctx->g->edgeCnt[u]; ++i) 
        res += dfs_a(ctx, ctx->g->edges[u][i]);

    ctx->memo[u] = res;
    return res;
}

void solve_day11a(FILE *input) {
    Graph g;
    graph_init(&g);
    read_graph(&g, input);

    int start = get_id(&g, "you");
    int out = get_id(&g, "out");

    DFS_A ctx = { .g = &g, .target = out };
    memset(ctx.used, 0, sizeof(ctx.used));

    printf("Res: %lld\n", dfs_a(&ctx, start));
}

typedef struct {
    ll memo[MAX_N][4];
    bool used[MAX_N][4];
    Graph* g;
    int out;
    int dac;
    int fft;
} DFS_B;

static ll dfs_b(DFS_B* ctx, int u, int state) {
    if (ctx->used[u][state]) return ctx->memo[u][state];
    ctx->used[u][state] = true;

    int newState = state;
    if (u == ctx->dac) newState |= 1; // dac
    if (u == ctx->fft) newState |= 2; // fft

    if (u == ctx->out) {
        ctx->memo[u][state] = (newState == 3 ? 1 : 0);
        return ctx->memo[u][state];
    }

    ll res = 0;
    for (int i = 0; i < ctx->g->edgeCnt[u]; ++i)
        res += dfs_b(ctx, ctx->g->edges[u][i], newState);

    ctx->memo[u][state] = res;
    return res;
}

void solve_day11b(FILE* input) {
    Graph g;
    graph_init(&g);
    read_graph(&g, input);

    int svr = get_id(&g, "svr");
    int out = get_id(&g, "out");
    int dac = get_id(&g, "dac");
    int fft = get_id(&g, "fft");

    DFS_B ctx = {
        .g = &g,
        .out = out,
        .dac = dac,
        .fft = fft
    };
    memset(ctx.used, 0, sizeof(ctx.used));

    printf("Res: %lld\n", dfs_b(&ctx, svr, 0));
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
        case 1: solve_day11a(input); break;
        case 2: solve_day11b(input); break;
    }

    if (input != stdin) fclose(input);
    return 0;
}
