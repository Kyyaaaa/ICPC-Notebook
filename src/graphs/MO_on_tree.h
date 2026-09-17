/*
   MO'S ALGORITHM ON TREES (WITH UPDATES - 3D MO)
   Idea:
   - Flattens the tree into an array using Euler Tour. Each node appears 
     exactly TWICE: when we enter it (tin) and when we leave it (tout).
   - A node is considered "in the current path" if it appears exactly ONCE 
     in our active range [L, R]. We maintain a boolean array `f[u]` to 
     toggle the presence of node `u`.
     
   Query Translation (Path from U to V, assuming tin[U] < tin[V]):
   - Case 1: U is an ancestor of V. 
             Query range: [tin[U], tin[V]]. LCA is already included.
   - Case 2: U is NOT an ancestor of V.
             Query range: [tout[U], tin[V]]. LCA is NOT in this range, 
             so we must manually add/remove the LCA during the query.
   Complexity:
   - Without updates (2D Mo): Block size = N / sqrt(Q), Time = O(N * sqrt(Q))
   - With updates (3D Mo): Block size = N^(2/3), Time = O(N^(5/3))
*/
const int N = 2e5 + 5;
// For 3D Mo (with updates), BLOCK size should be roughly N^(2/3). 
// For N = 100,000, N^(2/3) is approx 2150.
const int BLOCK = 2150; 

int n, q;
int a[N];
vector<int> G[N];

// Euler Tour & LCA
int tin[N], tout[N], timer;
int tour[N * 2]; // Needs to be 2 * N because each node appears twice
int up[18][N], depth[N];

// Mo's State
int freq[N], distinct_count;
int f[N]; // f[x] = 1 if node x is currently in the path, 0 otherwise
int ans[N];

struct Query {
  int l, r, lca_idx, t, id;
  
  bool operator<(const Query& other) const {
    int bl = l / BLOCK;
    int br = r / BLOCK;
    int obl = other.l / BLOCK;
    int obr = other.r / BLOCK;
    if (bl != obl) return bl < obl;
    if (br != obr) return (bl & 1) ? br < obr : br > obr;
    return (br & 1) ? t < other.t : t > other.t;
  }
};

struct Update {
  int x, new_val, old_val;
};

vector<Query> queries;
vector<Update> updates;

// 1. Build Euler Tour and LCA table
void dfs(int u, int par) {
  up[0][u] = par;
  for (int i = 1; i < 18; i++) {
    if (up[i - 1][u] != -1) {
      up[i][u] = up[i - 1][up[i - 1][u]];
    } else {
      up[i][u] = -1;
    }
  }
  
  tin[u] = ++timer;
  tour[timer] = u;
  
  for (int v : G[u]) {
    if (v != par) {
      depth[v] = depth[u] + 1;
      dfs(v, u);
    }
  }
  
  tout[u] = ++timer;
  tour[timer] = u;
}

int get_lca(int u, int v) {
  if (depth[u] < depth[v]) swap(u, v);
  int delta = depth[u] - depth[v];
  for (int i = 0; i < 18; i++) {
    if ((delta >> i) & 1) u = up[i][u];
  }
  if (u == v) return u;
  for (int i = 17; i >= 0; i--) {
    if (up[i][u] != up[i][v]) {
      u = up[i][u];
      v = up[i][v];
    }
  }
  return up[0][u];
}

// 2. State Manipulation Functions
void add(int val) {
  if (++freq[val] == 1) distinct_count++;
}

void sub(int val) {
  if (freq[val]-- == 1) distinct_count--;
}

// Toggle a node: if it's already in the path, remove it. Otherwise, add it.
void toggle(int idx) {
  int x = tour[idx];
  f[x] ^= 1;
  if (f[x]) add(a[x]);
  else sub(a[x]);
}

// Apply point update dynamically
void apply_update(int x, int val, int old_val) {
  if (f[x]) {
    add(val);
    sub(old_val);
  }
  a[x] = val;
}

/*
   USAGE EXAMPLE:
   
   signed main() {
     // ... Read N, Q, array `a`, and tree edges ...
     
     timer = 0;
     memset(up, -1, sizeof(up));
     depth[1] = 0;
     dfs(1, -1);
     
     // Note: If values are large, do Coordinate Compression on `a` and updates first!
     
     // ... Reading Queries and Updates ...
     int time_counter = 0;
     for (int i = 1; i <= q; i++) {
       int type; cin >> type;
       if (type == 1) { // Path Query
         int u, v; cin >> u >> v;
         if (tin[u] > tin[v]) swap(u, v); // Ensure tin[u] <= tin[v]
         
         int lca = get_lca(u, v);
         if (lca == u) {
           // u is ancestor of v -> LCA is already inside [tin[u], tin[v]]
           queries.push_back({tin[u], tin[v], -1, time_counter, (int)queries.size() + 1});
         } else {
           // u is NOT ancestor of v -> LCA is missing, need to add it manually
           queries.push_back({tout[u], tin[v], tin[lca], time_counter, (int)queries.size() + 1});
         }
       } else { // Point Update
         int x, val; cin >> x >> val;
         time_counter++;
         updates.push_back({x, val, a[x]});
         a[x] = val; // temporarily apply to get `old_val` for future queries
       }
     }
     
     // Rollback the array `a` to its initial state before processing Mo
     for (int i = (int)updates.size() - 1; i >= 0; i--) {
       a[updates[i].x] = updates[i].old_val;
     }
     
     sort(queries.begin(), queries.end());
     
     int L = 1, R = 0, T = 0;
     for (const auto& q : queries) {
       while (R < q.r) toggle(++R);
       while (L > q.l) toggle(--L);
       while (R > q.r) toggle(R--);
       while (L < q.l) toggle(L++);
       
       while (T < q.t) {
         apply_update(updates[T].x, updates[T].new_val, updates[T].old_val);
         T++;
       }
       while (T > q.t) {
         T--;
         apply_update(updates[T].x, updates[T].old_val, updates[T].new_val);
       }
       
       // Manually toggle LCA if needed
       if (q.lca_idx != -1) toggle(q.lca_idx);
       
       ans[q.id] = distinct_count;
       
       // Untoggle LCA to restore state
       if (q.lca_idx != -1) toggle(q.lca_idx);
     }
     
     for (int i = 1; i <= queries.size(); i++) {
       cout << ans[i] << "\n";
     }
     return 0;
   }
*/