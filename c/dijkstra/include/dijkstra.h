#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

int minDistance(const double dist[], const char done[], int n);
int dijkstra(const Graph *g, int src, double dist[], int parent[]);
void printPaths(const Graph *g, int src, const double dist[], const int parent[]);

#endif
