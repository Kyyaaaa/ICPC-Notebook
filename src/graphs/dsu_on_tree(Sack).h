const int N = 2e5 + 5;
vector<int> G[N];

int in[N], out[N], rn[N], timer;
int sz[N], big[N];

// 1. First DFS: Precompute subtree sizes, Euler tour, and heavy children
void dfs_sz(int u, int par) {
  in[u] = ++timer;
  rn[timer] = u;
  sz[u] = 1;
  big[u] = 0;
  
  for (int v : G[u]) {
    if (v != par) {
      dfs_sz(v, u);
      sz[u] += sz[v];
      if (sz[big[u]] < sz[v]) {
        big[u] = v; // Update heavy child
      }
    }
  }
  out[u] = timer;
}

// Struct to manage the global state (Modify this according to the problem!)
struct GlobalState {
  int freq[N];
  int count_of_freq[N];
  
  void add(int node) {
  }
  
  void remove(int node) {
  }
  
} state;


// 2. Second DFS: DSU on Tree (Sack)
void dsu_on_tree(int u, int par, bool keep) {
  for (int v : G[u]) {
    if (v != par && v != big[u]) {
      dsu_on_tree(v, u, false);
    }
  }
  
  if (big[u] != 0) {
    dsu_on_tree(big[u], u, true);
  }
  
  for (int v : G[u]) {
    if (v != par && v != big[u]) {
      for (int i = in[v]; i <= out[v]; i++) {
        state.add(rn[i]);
      }
    }
  }
  state.add(u);
  
  // ANSWER QUERIES for node `u` here
  
  if (!keep) {
    for (int i = in[u]; i <= out[u]; i++) {
      state.remove(rn[i]);
    }
  }
}