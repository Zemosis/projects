#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

// One line of the input file
typedef struct Edge {
    long u;
    long v;
    double w;
} Edge;

// Growable array of edges
typedef struct EdgeList {
    Edge *e;
    size_t m;
    size_t cap;
} EdgeList;

// Adjacency list in compressed sparse row (CSR) form.
// Neighbors of node i are adj[offset[i]] .. adj[offset[i + 1] - 1]
typedef struct Graph {
    int n;          // number of distinct nodes
    size_t m;       // number of edges read from the file
    int directed;
    long *label;    // label[i] = original id of node i
    size_t *offset;
    int *adj;       // neighbor node index
    double *wt;     // weight of the matching edge
} Graph;

int readEdgeList(const char *filename, EdgeList *el);
void buildGraph(EdgeList *el, int directed, Graph *g);
int findNodeIndex(const Graph *g, long label);
void freeGraph(Graph *g);

#endif
