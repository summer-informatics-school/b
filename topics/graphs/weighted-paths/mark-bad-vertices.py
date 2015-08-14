updated = [False] * vn
for u, v, w in edges:
  if relax_edge(u, v, w):
    mark[v] = True

used = [False] * vn
for v in range(vn):
  if updated[v]:
    dfs(v)