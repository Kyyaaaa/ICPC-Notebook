/*
   Idea:
   - Supports union of two sets, rolling back to a previous state, and 
     checking if the current graph is Bipartite (2-colorable).
   - STRICT RULE: NO PATH COMPRESSION! We use "Union by Size" only.
   - We maintain `parity[v]`: the color difference between `v` and `parent[v]`.
   - We track `bipartite_violations`: the number of odd cycles. The graph 
     is bipartite if and only if `bipartite_violations == 0`.
*/
struct BipartiteDSU_Rollback {
  vector<int> parent, sz, parity;
  int components;
  int bipartite_violations;
  
  // History structure to record every operation for accurate rollback
  struct Op {
    int child, parent;
    bool added_violation;
  };
  vector<Op> history;

  BipartiteDSU_Rollback(int n) {
    parent.assign(n + 1, 0);
    sz.assign(n + 1, 1);
    parity.assign(n + 1, 0); // 0 = same color as parent, 1 = different
    components = n;
    bipartite_violations = 0;
    
    for (int i = 1; i <= n; i++) {
      parent[i] = i;
    }
  }

  // Find set WITHOUT path compression.
  // Returns {root, color_relative_to_root}
  pair<int, int> find_set(int v) {
    int color = 0;
    while (v != parent[v]) {
      color ^= parity[v];
      v = parent[v];
    }
    return {v, color};
  }

  // Returns true if the entire graph can be 2-colored
  bool is_bipartite() {
    return bipartite_violations == 0;
  }

  // Union two sets and ensure they have DIFFERENT colors
  bool union_sets(int a, int b) {
    auto [root_a, color_a] = find_set(a);
    auto [root_b, color_b] = find_set(b);

    // If they are already in the same component
    if (root_a == root_b) {
      if (color_a == color_b) {
        // Same color -> Odd cycle created -> Bipartite violated
        bipartite_violations++;
        history.push_back({-1, -1, true});
      } else {
        // Even cycle -> Safe, no structural changes needed
        history.push_back({-1, -1, false});
      }
      return false; // No component merged
    }

    // Union by size
    if (sz[root_a] > sz[root_b]) {
      swap(root_a, root_b);
      swap(color_a, color_b);
    }

    // Perform union
    parent[root_a] = root_b;
    sz[root_b] += sz[root_a];
    components--;
    
    // Set parity so that 'a' and 'b' have different colors
    // We want: color_a ^ parity[root_a] ^ color_b == 1
    parity[root_a] = color_a ^ color_b ^ 1;
    
    // Record the operation
    history.push_back({root_a, root_b, false});
    return true;
  }

  // Take a snapshot of the current state
  int snapshot() {
    return history.size();
  }

  // Rollback operations to the exact state at `checkpoint`
  void rollback(int checkpoint) {
    while (history.size() > checkpoint) {
      Op op = history.back();
      history.pop_back();

      // Undo bipartite violation if this operation caused one
      if (op.added_violation) {
        bipartite_violations--;
      }
      
      // Undo structural changes if a union actually happened
      if (op.child != -1) {
        int a = op.child;
        int b = op.parent;
        
        sz[b] -= sz[a];
        parent[a] = a;
        parity[a] = 0; // Restore parity
        components++;
      }
    }
  }
};