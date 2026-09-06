const long long LINF = 1e18;
const int N = 505;

long long dist[N][N];
int nxt[N][N];

void init(int n) {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      dist[i][j] = LINF;
      nxt[i][j] = -1;
    }
    dist[i][i] = 0;
  }
}

void add_edge(int u, int v, long long w) {
  if (w < dist[u][v]) {
    dist[u][v] = w;
    nxt[u][v] = v;
  }
}

void floyd_warshall(int n) {
  for (int k = 1; k <= n; k++) {
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (dist[i][k] < LINF && dist[k][j] < LINF) {
          if (dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
            nxt[i][j] = nxt[i][k];
          }
        }
      }
    }
  }
  
  // 4. (Optional) Propagate Negative Cycles
  // If node k is part of a negative cycle, any path going through k 
  // will also have a length of -Infinity.
  for (int k = 1; k <= n; k++) {
    if (dist[k][k] < 0) {
      for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
          if (dist[i][k] < LINF && dist[k][j] < LINF) {
            dist[i][j] = -LINF; 
          }
        }
      }
    }
  }
}

// 5. Path Reconstruction
// Returns the path from u to v, or an empty vector if no path exists
vector<int> get_path(int u, int v) {
  if (dist[u][v] == LINF) return {};
  
  vector<int> path;
  for (int at = u; at != v; at = nxt[at][v]) {
    if (at == -1) return {};
    path.push_back(at);
  }
  path.push_back(v);
  return path;
}