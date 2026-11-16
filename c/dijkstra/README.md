# Dijkstra's Algorithm (C)

Single-source shortest paths for the CS 483 project. Reads a weighted graph from an
edge-list file and prints the shortest path from a start node to every reachable node.

## Build

    make

## Run

    ./dijkstra <graph-file> <directed|undirected> <start-node>

The graph file has one edge per line, `u v w` separated by white space
(two node ids and a weight). Node ids can be 0-based, 1-based or sparse.

    ./dijkstra tests/small_undirected.txt undirected 1
    ./dijkstra tests/small_directed.txt directed 0

## Files

- `main.c` command-line handling and driver
- `graph.c / include/graph.h` reads the edge list and builds the adjacency list
- `dijkstra.c / include/dijkstra.h` the algorithm and the output
- `util.c / include/util.h` small helpers
- `tools/gen_graph.py` generates random test graphs
- `tools/verify.py` checks the output against a Python heapq reference

## Status

Works with a linear scan for the closest node (O(n^2)). Next: a min-heap,
printing the dist/parent/heap arrays after every iteration, and input validation.
