struct DSU_Rollback {
  vector<int> parent, sz;
  vector<pair<int, int>> history;

  DSU_Rollback(int n) {
    parent.assign(n + 1, 0);
    sz.assign(n + 1, 1);
    for (int i = 1; i <= n; i++) parent[i] = i;
  }

  int find_set(int v) {
    while (v != parent[v]) v = parent[v];
    return v;
  }

  bool union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a == b) return false;
    
    if (sz[a] > sz[b]) swap(a, b);
    parent[a] = b;
    sz[b] += sz[a];
    history.push_back({a, b});
    return true;
  }

  int snapshot() {
    return history.size();
  }

  void rollback(int checkpoint) {
    while (history.size() > checkpoint) {
      int a = history.back().first;
      int b = history.back().second;
      history.pop_back();
      
      sz[b] -= sz[a];
      parent[a] = a;
    }
  }
};

struct DynamicConnectivity {
  int max_time;
  DSU_Rollback dsu;
  vector<vector<pair<int, int>>> tree;
  vector<vector<pair<int, int>>> queries; // queries[time] = list of {u, v}
  vector<int> ans; // stores answer (1 or 0) for each query sequentially

  DynamicConnectivity(int n, int _max_time) : max_time(_max_time), dsu(n) {
    tree.assign(4 * max_time + 1, vector<pair<int, int>>());
    queries.assign(max_time + 1, vector<pair<int, int>>());
  }

  void add_edge_interval(int node, int l, int r, int ql, int qr, pair<int, int> edge) {
    if (ql > r || qr < l) return;
    if (ql <= l && r <= qr) {
      tree[node].push_back(edge);
      return;
    }
    int mid = l + (r - l) / 2;
    add_edge_interval(2 * node, l, mid, ql, qr, edge);
    add_edge_interval(2 * node + 1, mid + 1, r, ql, qr, edge);
  }

  // Add an edge that exists from start_time to end_time
  void add_edge(int u, int v, int start_time, int end_time) {
    if (start_time > end_time) return;
    add_edge_interval(1, 1, max_time, start_time, end_time, {u, v});
  }

  // Register a connectivity query at a specific time
  void add_query(int u, int v, int time) {
    queries[time].push_back({u, v});
  }

  // Traverse the time tree and answer queries
  void dfs(int node, int l, int r) {
    int checkpoint = dsu.snapshot();
    
    // Apply all edges covering this node
    for (auto edge : tree[node]) {
      dsu.union_sets(edge.first, edge.second);
    }

    if (l == r) {
      // Leaf node: Answer queries exactly at time 'l'
      for (auto q : queries[l]) {
        ans.push_back(dsu.find_set(q.first) == dsu.find_set(q.second));
      }
    } else {
      // Internal node: Recurse left and right
      int mid = l + (r - l) / 2;
      dfs(2 * node, l, mid);
      dfs(2 * node + 1, mid + 1, r);
    }

    // Rollback the DSU state before going back up
    dsu.rollback(checkpoint);
  }

  // Solves and returns a vector containing answers to all queries
  vector<int> solve() {
    if (max_time >= 1) {
      dfs(1, 1, max_time);
    }
    return ans;
  }
};

/*
   USAGE EXAMPLE (Handling Map for Edge Lifespans):
   
   signed main() {
     int n = 5;  // 5 nodes
     int q = 6;  // 6 events/queries
     
     DynamicConnectivity dc(n, q);
     map<pair<int, int>, int> edge_start;
     
     // Note: Always normalize edges so u < v to avoid {u, v} and {v, u} mix-ups.
     auto normalize = [](int u, int v) {
       return u < v ? make_pair(u, v) : make_pair(v, u);
     };
     
     // Simulating reading queries:
     // Time 1: Add edge (1, 2)
     edge_start[normalize(1, 2)] = 1;
     
     // Time 2: Add edge (2, 3)
     edge_start[normalize(2, 3)] = 2;
     
     // Time 3: Query (1, 3) -> should be connected
     dc.add_query(1, 3, 3);
     
     // Time 4: Remove edge (1, 2)
     pair<int, int> e = normalize(1, 2);
     dc.add_edge(e.first, e.second, edge_start[e], 4 - 1); 
     edge_start.erase(e); // Edge doesn't exist after time 3
     
     // Time 5: Query (1, 3) -> should NOT be connected
     dc.add_query(1, 3, 5);
     
     // Time 6: Add edge (1, 3)
     edge_start[normalize(1, 3)] = 6;
     
     // IMPORTANT: Close any edges that were never removed!
     for (auto it : edge_start) {
       dc.add_edge(it.first.first, it.first.second, it.second, q);
     }
     
     // Solve and print
     vector<int> results = dc.solve();
     for (int res : results) {
       cout << (res ? "YES" : "NO") << "\n";
     }
     
     // Expected Output:
     // YES (at time 3)
     // NO (at time 5)
     
     return 0;
   }
*/