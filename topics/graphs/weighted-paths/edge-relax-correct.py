def edge_relax(u, v, w):
  if dist[u] < +INF and dist[v] > dist[u] + w:
    dist[v] = max(-INF, dist[u] + w)
    prev[u] = v
    return True
  return False
