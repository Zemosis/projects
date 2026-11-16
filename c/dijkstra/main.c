// CS 483 Project - Dijkstra's Algorithm (Milestone #2, work in progress)
//
// Usage: ./dijkstra <graph-file> <directed|undirected> <start-node>
//
// The input file is an edge list, one "u v w" per line. Prints every valid
// shortest path from the start node with its distance.
//
// TODO: min-heap, per-iteration dist/parent/heap output, input validation,
// large-graph testing

#include "dijkstra.h"
#include "graph.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Prototype
static void usage(const char *prog);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    int directed;
    if (sameWord(argv[2], "directed") || sameWord(argv[2], "d")) {
        directed = 1;
    } else if (sameWord(argv[2], "undirected") || sameWord(argv[2], "u")) {
        directed = 0;
    } else {
        fprintf(stderr,
                "error: graph type must be \"directed\" or \"undirected\", got \"%s\"\n",
                argv[2]);
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    char *end;
    long startLabel = strtol(argv[3], &end, 10);
    if (end == argv[3] || *end != '\0') {
        fprintf(stderr, "error: start node must be an integer, got \"%s\"\n", argv[3]);
        return EXIT_FAILURE;
    }

    EdgeList el;
    if (readEdgeList(argv[1], &el) != 0) {
        return EXIT_FAILURE;
    }
    Graph g;
    buildGraph(&el, directed, &g);
    free(el.e);

    int src = findNodeIndex(&g, startLabel);
    if (src < 0) {
        fprintf(stderr, "error: start node %ld does not appear in %s\n", startLabel,
                argv[1]);
        freeGraph(&g);
        return EXIT_FAILURE;
    }

    printf("Input file : %s\n", argv[1]);
    printf("Graph      : %s, %d nodes, %zu edges\n",
           directed ? "directed" : "undirected", g.n, g.m);
    printf("Start node : %ld\n", startLabel);

    double *dist = safeMalloc((size_t)g.n * sizeof(double));
    int *parent = safeMalloc((size_t)g.n * sizeof(int));

    clock_t t0 = clock();
    int settled = dijkstra(&g, src, dist, parent);
    clock_t t1 = clock();

    printPaths(&g, src, dist, parent);
    fflush(stdout);
    fprintf(stderr, "dijkstra: settled %d of %d nodes in %.3f s (CPU)\n", settled,
            g.n, (double)(t1 - t0) / CLOCKS_PER_SEC);

    free(dist);
    free(parent);
    freeGraph(&g);
    return EXIT_SUCCESS;
}

static void usage(const char *prog) {
    fprintf(stderr,
            "usage: %s <graph-file> <directed|undirected> <start-node>\n"
            "  graph-file   edge list, one \"u v w\" per line\n",
            prog);
}
