#!/usr/bin/env python3
"""Generate a random weighted edge list:  gen_graph.py <nodes> <edges> <out-file> [seed]
Nodes are 0..nodes-1; a random spanning chain is included so most nodes are reachable."""
import random, sys

n, m, out = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3]
rng = random.Random(int(sys.argv[4]) if len(sys.argv) > 4 else 483)
order = list(range(n)); rng.shuffle(order)
with open(out, "w") as f:
    for i in range(1, n):                       # chain keeps the graph connected
        f.write(f"{order[i-1]} {order[i]} {rng.randint(1, 100)}\n")
    for _ in range(max(0, m - (n - 1))):
        f.write(f"{rng.randrange(n)} {rng.randrange(n)} {rng.randint(1, 100)}\n")
