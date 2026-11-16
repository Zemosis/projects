#include "dijkstra.h"
#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Returns the unvisited node with the smallest finite distance, or -1 if every
// remaining node is unreachable. O(n) per call.
// Milestone #1 used "<=" here, which picks a node at INFINITY once nothing
// reachable is left, so the u == -1 stop never fired. Changed to "<".
int minDistance(const double dist[], const char done[], int n) {
    double min = INFINITY;
    int minIndex = -1;
    for (int v = 0; v < n; v++) {
        if (!done[v] && dist[v] < min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

// Fills dist[] and parent[] (node indices, -1 = none) for every node.
// Returns the number of nodes settled (= reachable nodes). O(n^2 + m).
// TODO: replace minDistance() with a min-heap and print dist/parent/heap
// after every iteration
int dijkstra(const Graph *g, int src, double dist[], int parent[]) {
    char *done = safeCalloc((size_t)g->n, 1);

    for (int v = 0; v < g->n; v++) {
        dist[v] = INFINITY;
        parent[v] = -1;
    }
    dist[src] = 0.0;

    // Settle one node per pass
    int iter;
    for (iter = 0; iter < g->n; iter++) {
        int u = minDistance(dist, done, g->n);
        if (u == -1) {
            break; // the rest is unreachable
        }
        done[u] = 1;

        // Relax u's edges
        for (size_t k = g->offset[u]; k < g->offset[u + 1]; k++) {
            int v = g->adj[k];
            double newDist = dist[u] + g->wt[k];
            if (!done[v] && newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
            }
        }
    }

    free(done);
    return iter;
}

// Prints every reachable node's shortest path from src, then the unreachable ones
void printPaths(const Graph *g, int src, const double dist[], const int parent[]) {
    int *path = safeMalloc((size_t)g->n * sizeof(int));
    int reachable = 0;
    int unreachable = 0;
    char buf[64];

    printf("\nShortest paths from node %ld:\n", g->label[src]);
    printf("  %-12s %-12s %s\n", "Destination", "Distance", "Path");

    for (int v = 0; v < g->n; v++) {
        if (isinf(dist[v])) {
            unreachable++;
            continue;
        }
        reachable++;

        // Walk back to the source (a loop, not recursion, so long paths
        // can't overflow the stack)
        int len = 0;
        for (int x = v; x != -1; x = parent[x]) {
            path[len++] = x;
        }

        // Print it forwards
        formatDist(dist[v], buf, sizeof buf);
        printf("  %-12ld %-12s ", g->label[v], buf);
        while (len > 0) {
            printf("%ld", g->label[path[--len]]);
            if (len > 0) {
                printf(" -> ");
            }
        }
        printf("\n");
    }

    printf("\n%d of %d nodes reachable from node %ld", reachable, g->n,
           g->label[src]);
    if (unreachable > 0 && unreachable <= 20) {
        printf("; unreachable:");
        for (int v = 0; v < g->n; v++) {
            if (isinf(dist[v])) {
                printf(" %ld", g->label[v]);
            }
        }
    } else if (unreachable > 0) {
        printf("; %d unreachable", unreachable);
    }
    printf("\n");
    free(path);
}
