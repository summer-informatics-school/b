for k in range(vn):
  for u in range(vn):
    for v in range(vn):
      if dist[u][v] > dist[u][k] + dist[k][v]:
        dist[u][v] = dist[u][k] + dist[k][v]
