struct DSU_Rollback {
  vector<int> parent, sz;
  int components;
  
  // Stores the history of unions as pairs {child, parent}
  vector<pair<int, int>> history;

  DSU_Rollback(int n) {
    parent.assign(n + 1, 0);
    sz.assign(n + 1, 1);
    components = n;
    for (int i = 1; i <= n; i++) {
      parent[i] = i;
    }
  }

  // Find set WITHOUT path compression
  int find_set(int v) {
    while (v != parent[v]) {
      v = parent[v];
    }
    return v;
  }

  // Union two sets by size and record the operation
  bool union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a == b) return false;

    // Union by size: always attach the smaller tree (a) to the larger one (b)
    if (sz[a] > sz[b]) {
      swap(a, b);
    }

    // Perform union
    parent[a] = b;
    sz[b] += sz[a];
    components--;
    
    // Record the operation: 'a' became a child of 'b'
    history.push_back({a, b});
    return true;
  }

  // Take a snapshot of the current state (returns the size of the history stack)
  int snapshot() {
    return history.size();
  }

  // Rollback operations until the history stack size is equal to `checkpoint`
  void rollback(int checkpoint) {
    while (history.size() > checkpoint) {
      int a = history.back().first;
      int b = history.back().second;
      history.pop_back();

      // Undo the union operation
      sz[b] -= sz[a];
      parent[a] = a;
      components++;
    }
  }
};

/*
   USAGE EXAMPLE (Dynamic Connectivity / Divide & Conquer on Time):
   
   signed main() {
     int n = 5;
     DSU_Rollback dsu(n);
     
     dsu.union_sets(1, 2);
     dsu.union_sets(3, 4);
     cout << "Components: " << dsu.components << "\n"; // Output: 3
     
     // 1. Save the current state
     int checkpoint = dsu.snapshot();
     
     // 2. Do some temporary unions
     dsu.union_sets(2, 3);
     dsu.union_sets(4, 5);
     cout << "Components after temp unions: " << dsu.components << "\n"; // Output: 1
     
     // 3. Rollback to the saved state
     dsu.rollback(checkpoint);
     
     // The state is perfectly restored!
     cout << "Components after rollback: " << dsu.components << "\n"; // Output: 3
     
     return 0;
   }
*/