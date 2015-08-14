for u in range(vn):
  for v in range(vn):
    for k in range(vn):
      if dist[k][k] < 0 and dist[u][k] < +INF \
                        and dist[k][v] < +INF:
        mark[u][v] = True
