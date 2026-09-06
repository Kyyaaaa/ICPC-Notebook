const long long LINF = 1e18;

struct Edge {
  int u, v;
  long long w;
};

int n, m;
vector<Edge> edges;
long long dist[2005];
int parent[2005];

bool bellman_ford(int src) {
  for (int i = 1; i <= n; i++) {
    dist[i] = LINF;
    parent[i] = -1;
  }
  dist[src] = 0;
  for (int i = 1; i <= n - 1; i++) {
    bool updated = false;
    for (auto e : edges) {
      if (dist[e.u] < LINF && dist[e.u] + e.w < dist[e.v]) {
        dist[e.v] = dist[e.u] + e.w;
        parent[e.v] = e.u;
        updated = true;
      }
    }
    if (!updated) break;
  }
  bool has_negative_cycle = false;
  for (int i = 1; i <= n; i++) {
    for (auto e : edges) {
      if (dist[e.u] < LINF && dist[e.u] + e.w < dist[e.v]) {
        dist[e.v] = -LINF;
        has_negative_cycle = true;
      }
    }
  }
  return has_negative_cycle;
}

/*
   HOW TO EXTRACT A NEGATIVE CYCLE (If needed)
   If you only need to print ONE negative cycle (like CSES Cycle Finding):
   1. Keep an array `parent`.
   2. Run Bellman-Ford for exactly N iterations (no need for Phase 2 above).
   3. On the N-th iteration, if an edge (u, v) is relaxed, `v` is either in 
      the negative cycle or reachable from it.
   4. To find a node strictly inside the cycle, go back `N` times using `parent`:
      int x = v;
      for (int i = 1; i <= n; i++) x = parent[x];
   5. Now `x` is guaranteed to be in the cycle. Trace back using `parent[x]` 
      until you reach `x` again, push nodes to a vector, and reverse it.
*/