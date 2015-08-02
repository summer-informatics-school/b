def edge_relax(u, v, w):
  if dist[v] > dist[u] + w:
    dist[v] = dist[u] + w
    prev[u] = v
    return True
  return False