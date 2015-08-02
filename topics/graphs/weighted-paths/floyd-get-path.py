path = []
while v != u:
  path.append(v)
  v = prev[u][v]
path.append(u)
path.reverse()
