#!/usr/bin/env python3
"""Check dijkstra's output against an independent heapq implementation.

Usage: verify.py <graph-file> <directed|undirected> <start> <dijkstra-output-file>
"""

import heapq
import sys
from collections import defaultdict

path, kind, outfile = sys.argv[1], sys.argv[2].lower(), sys.argv[4]
start = int(sys.argv[3])
directed = kind in ("directed", "d")

# Read the graph
adj = defaultdict(list)
with open(path) as f:
    for line in f:
        t = line.split()
        if not t or t[0][0] in "#%":
            continue
        u, v = int(t[0]), int(t[1])
        w = float(t[2]) if len(t) > 2 else 1.0
        adj[u].append((v, w))
        if not directed and u != v:
            adj[v].append((u, w))

# Reference Dijkstra with a lazy heap
dist = {start: 0.0}
pq = [(0.0, start)]
while pq:
    d, u = heapq.heappop(pq)
    if d > dist[u]:
        continue
    for v, w in adj[u]:
        if d + w < dist.get(v, float("inf")):
            dist[v] = d + w
            heapq.heappush(pq, (d + w, v))

# Parse the "Shortest paths" table from the program output
got = {}
bad = 0
with open(outfile) as f:
    reading = False
    for line in f:
        if line.startswith("  Destination"):
            reading = True
            continue
        if not reading:
            continue
        t = line.split()
        if not t:
            break
        node, d = int(t[0]), float(t[1])
        nodes = [int(x) for x in t[2:] if x != "->"]
        got[node] = d
        if nodes[0] != start or nodes[-1] != node:
            bad += 1

if set(got) != set(dist):
    print("FAIL: reachable sets differ")
    sys.exit(1)

bad += sum(abs(got[v] - dist[v]) > 1e-6 for v in dist)
status = "OK" if bad == 0 else f"FAIL: {bad} mismatches"
print(status, f"({len(dist)} reachable nodes checked)")
sys.exit(bad != 0)
