#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_EDGES 1000

typedef long long ll;

typedef struct {
    int x, y, z;
} Point3D;

typedef struct {
    int u, v;
    ll dist;
} Edge;

typedef struct {
    int* parents;
    int* size;
    int n;
    int components;
} UnionFind;

UnionFind uf_create(int n) {
    UnionFind uf;
    uf.n = n;
    uf.components = n;
    uf.parents = malloc(n * sizeof(int));
    uf.size = malloc(n * sizeof(int));

    for (int i = 0; i < n; ++i) {
        uf.parents[i] = i;
        uf.size[i] = 1;
    }

    return uf;
}

int uf_find(UnionFind* uf, int x) {
    while (uf->parents[x] != x) {
        uf->parents[x] = uf->parents[uf->parents[x]];
        x = uf->parents[x];
    }
    return x;
}

void uf_union(UnionFind* uf, int u, int v) {
    u = uf_find(uf, u);
    v = uf_find(uf, v);
    if (u == v) return;

    if (uf->size[u] < uf->size[v]) {
        int tmp = u;
        u = v;
        v = tmp;
    }

    uf->parents[v] = u;
    uf->size[u] += uf->size[v];
    uf->components--;
}

void uf_free(UnionFind* uf) {
    free(uf->parents);
    free(uf->size);
}

static inline ll squared_dist(const Point3D* a, const Point3D* b) {
    ll dx = a->x - b->x;
    ll dy = a->y - b->y;
    ll dz = a->z - b->z;
    return dx*dx + dy*dy + dz*dz;
}

static inline ll minll(ll a, ll b) {
    return (a < b) ? a : b;
}

int read_points(FILE* input, Point3D** output) {
    int n = 0, cap = 0;
    Point3D* pts = NULL;

    while (1) {
        int x, y, z;
        if (fscanf(input, "%d,%d,%d", &x, &y, &z) != 3) break;
        if (n >= cap) {
            cap = cap ? cap * 2 : 1024;
            pts = realloc(pts, cap * sizeof(Point3D));
        }
        pts[n++] = (Point3D){x, y, z};
    }

    *output = pts;
    return n;
}

Edge* build_edges(const Point3D* pts, int n, ll* edgesOutCnt) {
    ll edgesCnt = (ll)n * (n - 1) / 2;
    Edge* edges = malloc(edgesCnt * sizeof(Edge));

    ll idx = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            edges[idx++] = (Edge){i, j, squared_dist(&pts[i], &pts[j])};
        }
    }

    *edgesOutCnt = edgesCnt;
    return edges;
}

static inline int cmp_edges(const void* a, const void* b) {
    const Edge* A = a;
    const Edge* B = b;
    return (A->dist > B->dist) - (A->dist < B->dist);
}


int* compute_component_sizes(UnionFind* uf) {
    int* sizes = calloc(uf->n, sizeof(int));
    for (int i = 0; i < uf->n; ++i) {
        int root = uf_find(uf, i);
        sizes[root]++;
    }
    return sizes;
}

static inline ll three_largest_product(int* arr, int n) {
    int a = 0, b = 0, c = 0;
    for (int i = 0; i < n; ++i) {
        int v = arr[i];
        if (v > a) { c = b; b = a; a = v; }
        else if (v > b) { c = b; b = v; }
        else if (v > c) { c = v; }
    }
    return (ll)a * b * c;
}


void solve_day08a(FILE *input) {
    Point3D* pts;
    int n = read_points(input, &pts);
    if (n == 0) { free(pts); return; }

    ll edgesCnt;
    Edge* edges = build_edges(pts, n, &edgesCnt);
    qsort(edges, edgesCnt, sizeof(Edge), cmp_edges);

    UnionFind uf = uf_create(n);

    ll limit = minll(edgesCnt, MAX_EDGES);
    for (ll i = 0; i < limit; ++i)
        uf_union(&uf, edges[i].u, edges[i].v);

    int* compSizes = compute_component_sizes(&uf);
    printf("Res: %lld\n", three_largest_product(compSizes, n));

    free(pts);
    free(edges);
    free(compSizes);
    uf_free(&uf);
}

static inline ll multiply_x_coords(const Point3D* pts, const Edge* e) { return (ll)pts[e->u].x * pts[e->v].x; }

void solve_day08b(FILE* input) {
    Point3D* pts;
    int n = read_points(input, &pts);
    if (n == 0) { free(pts); return; }

    ll edgesCnt;
    Edge* edges = build_edges(pts, n, &edgesCnt);
    qsort(edges, edgesCnt, sizeof(Edge), cmp_edges);

    UnionFind uf = uf_create(n);
    Edge lastEdge = {0};

    for (ll i = 0; i < edgesCnt; ++i) {
        int u = uf_find(&uf, edges[i].u);
        int v = uf_find(&uf, edges[i].v);
        if (u != v) {
            uf_union(&uf, u, v);
            lastEdge = edges[i];
        }
        if (uf.components == 1) break;
    }

    printf("Res: %lld\n", multiply_x_coords(pts, &lastEdge));

    free(pts);
    free(edges);
    uf_free(&uf);
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
        case 1: solve_day08a(input); break;
        case 2: solve_day08b(input); break;
    }

    if (input != stdin) fclose(input);
    return 0;
}
