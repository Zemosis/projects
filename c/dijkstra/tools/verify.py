#!/usr/bin/env python3
"""Check dijkstra's output against an independent heapq implementation.
   verify.py <graph-file> <directed|undirected> <start> <dijkstra-output-file>"""
import heapq, sys
from collections import defaultdict

path, kind, start, outfile = sys.argv[1], sys.argv[2].lower(), int(sys.argv[3]), sys.argv[4]
directed = kind in ("directed", "d")
adj = defaultdict(list)
for line in open(path):
    t = line.split()
    if not t or t[0][0] in "#%": continue
    u, v, w = int(t[0]), int(t[1]), float(t[2]) if len(t) > 2 else 1.0
    adj[u].append((v, w))
    if not directed and u != v: adj[v].append((u, w))
dist, pq = {start: 0.0}, [(0.0, start)]
while pq:
    d, u = heapq.heappop(pq)
    if d > dist[u]: continue
    for v, w in adj[u]:
        if d + w < dist.get(v, float("inf")):
            dist[v] = d + w; heapq.heappush(pq, (d + w, v))

got, bad = {}, 0
reading = False
for line in open(outfile):
    if line.startswith("  Destination"): reading = True; continue
    if reading:
        t = line.split()
        if not t: break
        node, d, p = int(t[0]), float(t[1]), [int(x) for x in t[2:] if x != "->"]
        got[node] = d
        if p[0] != start or p[-1] != node: bad += 1
if set(got) != set(dist): print("FAIL: reachable sets differ"); sys.exit(1)
bad += sum(abs(got[v] - dist[v]) > 1e-6 for v in dist)
print("OK" if bad == 0 else f"FAIL: {bad} mismatches", f"({len(dist)} reachable nodes checked)")
sys.exit(bad != 0)
