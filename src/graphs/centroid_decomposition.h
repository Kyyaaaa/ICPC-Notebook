const int N = 3e5 + 5;
vector<int> G[N];
int sz[N];
bool deleted[N];
int centroid_par[N];

void get_sz(int u, int par) {
  sz[u] = 1;
  for (int v : G[u]) {
    if (v != par && !deleted[v]) {
      get_sz(v, u);
      sz[u] += sz[v];
    }
  }
}

int get_centroid(int u, int par, int total_nodes) {
  for (int v : G[u]) {
    if (v != par && !deleted[v]) {
      if (sz[v] > total_nodes / 2) {
        return get_centroid(v, u, total_nodes);
      }
    }
  }
  return u;
}

void solve_centroid(int centroid) {
  // TODO: Add your logic here 
}

void build_centroid(int u, int p = -1) {
  get_sz(u, -1);
  int centroid = get_centroid(u, -1, sz[u]);
  deleted[centroid] = true;
  centroid_par[centroid] = p; 
  solve_centroid(centroid);
  for (int v : G[centroid]) {
    if (!deleted[v]) {
      build_centroid(v, centroid);
    }
  }
}