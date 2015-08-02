dist = [+INF] * vn
dist[s] = 0
for i in range(vn - 1):
  for u, v, w in edges:
    relax_edge(u, v, w)