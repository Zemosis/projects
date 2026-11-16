#include "graph.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype
static int cmpLong(const void *a, const void *b);
static void addEdge(EdgeList *el, long u, long v, double w);

// Reads "u v w" triples into el. Returns 0 on success, -1 on error.
// TODO: skip comment lines, reject negative weights, line numbers in errors
int readEdgeList(const char *filename, EdgeList *el) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return -1;
    }

    el->m = 0;
    el->cap = 1024;
    el->e = safeMalloc(el->cap * sizeof(Edge));

    long u, v;
    double w;
    int got;
    while ((got = fscanf(fp, "%ld %ld %lf", &u, &v, &w)) == 3) {
        addEdge(el, u, v, w);
    }
    fclose(fp);

    if (got != EOF) {
        fprintf(stderr,
                "error: %s: could not parse edge #%zu (expected \"u v w\")\n",
                filename, el->m + 1);
        return -1;
    }
    if (el->m == 0) {
        fprintf(stderr, "error: %s contains no edges\n", filename);
        return -1;
    }
    return 0;
}

// Builds the CSR adjacency list. Node labels are sorted and de-duplicated so
// node index i <-> label[i]. An undirected edge is stored in both directions
// (a self-loop only once). Overwrites el's u, v with node indices.
void buildGraph(EdgeList *el, int directed, Graph *g) {
    // Collect every label, sort, and remove duplicates
    long *all = safeMalloc(2 * el->m * sizeof(long));
    for (size_t i = 0; i < el->m; i++) {
        all[2 * i] = el->e[i].u;
        all[2 * i + 1] = el->e[i].v;
    }
    qsort(all, 2 * el->m, sizeof(long), cmpLong);

    int n = 0;
    for (size_t i = 0; i < 2 * el->m; i++) {
        if (i == 0 || all[i] != all[i - 1]) {
            all[n++] = all[i];
        }
    }

    g->n = n;
    g->m = el->m;
    g->directed = directed;
    g->label = safeMalloc((size_t)n * sizeof(long));
    memcpy(g->label, all, (size_t)n * sizeof(long));
    free(all);

    // Relabel edges and count each node's out-degree
    g->offset = safeCalloc((size_t)n + 1, sizeof(size_t));
    for (size_t i = 0; i < el->m; i++) {
        el->e[i].u = findNodeIndex(g, el->e[i].u);
        el->e[i].v = findNodeIndex(g, el->e[i].v);
        g->offset[el->e[i].u + 1]++;
        if (!directed && el->e[i].u != el->e[i].v) {
            g->offset[el->e[i].v + 1]++;
        }
    }

    // Prefix sums give the start of each node's neighbor block
    for (int k = 1; k <= n; k++) {
        g->offset[k] += g->offset[k - 1];
    }
    size_t total = g->offset[n];

    // Place every edge into its block
    g->adj = safeMalloc(total * sizeof(int));
    g->wt = safeMalloc(total * sizeof(double));
    size_t *fill = safeMalloc((size_t)n * sizeof(size_t));
    memcpy(fill, g->offset, (size_t)n * sizeof(size_t));

    for (size_t i = 0; i < el->m; i++) {
        int u = (int)el->e[i].u;
        int v = (int)el->e[i].v;
        double w = el->e[i].w;

        g->adj[fill[u]] = v;
        g->wt[fill[u]++] = w;
        if (!directed && u != v) {
            g->adj[fill[v]] = u;
            g->wt[fill[v]++] = w;
        }
    }
    free(fill);
}

// Binary search for a label, returns its node index or -1 if not found
int findNodeIndex(const Graph *g, long label) {
    int lo = 0;
    int hi = g->n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (g->label[mid] == label) {
            return mid;
        }
        if (g->label[mid] < label) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

void freeGraph(Graph *g) {
    free(g->label);
    free(g->offset);
    free(g->adj);
    free(g->wt);
}

// qsort comparator for longs
static int cmpLong(const void *a, const void *b) {
    long x = *(const long *)a;
    long y = *(const long *)b;
    return (x > y) - (x < y);
}

// Appends one edge, doubling the array when it is full
static void addEdge(EdgeList *el, long u, long v, double w) {
    if (el->m == el->cap) {
        el->cap *= 2;
        el->e = safeRealloc(el->e, el->cap * sizeof(Edge));
    }
    el->e[el->m].u = u;
    el->e[el->m].v = v;
    el->e[el->m].w = w;
    el->m++;
}
