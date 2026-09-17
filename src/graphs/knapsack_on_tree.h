/*
   DP KNAPSACK ON TREE (Prerequisite DP / Connected Component Knapsack)
   Idea:
   - Problem: Choose a subset of nodes to maximize total value such that the 
     total weight <= W. 
     CONSTRAINT: If a node is chosen, its parent MUST also be chosen. 
     (e.g., Course selection with prerequisites, or picking a connected 
     component rooted at node 1).
   - Naive DP merges subtrees taking O(N * W^2).
   OPTIMIZATION (DFS Flattening Trick): 
   - We flatten the tree using DFS Pre-Order traversal. 
   - For a node at pre-order index `i`, its entire subtree spans contiguously 
     from index `i` to `i + sz[u] - 1`.
   - Transitions:
     1. Skip node `i`: We CANNOT pick any node in its subtree. We jump 
        directly to index `i + sz[u]`.
     2. Take node `i`: We pay `weight[u]`, gain `value[u]`, and move to 
        index `i + 1` (allowing us to pick its children).
   Complexity:
   - Time: O(N * W) 
   - Space: O(N * W)
*/
struct TreeKnapsack {
  int n;
  vector<vector<int>> adj;
  vector<long long> weight, value;
  
  // DFS state
  vector<int> sz;
  vector<int> rev_dfn; // rev_dfn[i] = node ID at DFS pre-order index 'i'
  int timer;

  TreeKnapsack(int _n) {
    n = _n;
    adj.assign(n + 1, vector<int>());
    weight.assign(n + 1, 0);
    value.assign(n + 1, 0);
    sz.assign(n + 1, 0);
    rev_dfn.assign(n + 1, 0);
    timer = 0;
  }

  // Add directed edge from parent to child
  void add_edge(int u, int v) {
    adj[u].push_back(v);
  }

  // Build the Pre-Order array and calculate subtree sizes
  void dfs(int u) {
    timer++;
    rev_dfn[timer] = u;
    sz[u] = 1;
    
    for (int v : adj[u]) {
      dfs(v);
      sz[u] += sz[v];
    }
  }

  // Solves the knapsack problem for a specific weight limit `W`
  long long solve(int root, int W) {
    timer = 0;
    dfs(root); // Flatten the tree

    // dp[i][w] = max value considering nodes in pre-order from index `i` to `n`,
    // having exactly `w` weight capacity available.
    // We use size (n + 2) to safely handle the jump to `n + 1` when skipping the last nodes.
    vector<vector<long long>> dp(n + 2, vector<long long>(W + 1, 0));

    // Iterate backwards through the pre-order indices
    for (int i = n; i >= 1; i--) {
      int u = rev_dfn[i];
      int jump = i + sz[u]; // The index to jump to if we skip subtree of `u`
      
      for (int w = 0; w <= W; w++) {
        // Option 1: Skip this node (and entirely skip its subtree)
        long long skip = dp[jump][w];
        
        // Option 2: Take this node (if current capacity allows)
        long long take = 0;
        if (w >= weight[u]) {
          take = dp[i + 1][w - weight[u]] + value[u];
        }
        
        dp[i][w] = max(skip, take);
      }
    }
    
    // The answer is the state starting at pre-order index 1 with full capacity W
    return dp[1][W];
  }
};

/*
   USAGE EXAMPLE:
   
   signed main() {
     // Suppose we have 5 nodes (1 is the root)
     int n = 5;
     TreeKnapsack tk(n);
     
     // Values and weights for nodes 1 to 5
     tk.weight = {0, 1, 2, 3, 2, 1}; // 1-indexed (index 0 is unused)
     tk.value  = {0, 5, 10, 12, 4, 3};
     
     // Tree structure: 1 is parent of 2 and 3. 2 is parent of 4 and 5.
     // Directed edges from parent to child!
     tk.add_edge(1, 2);
     tk.add_edge(1, 3);
     tk.add_edge(2, 4);
     tk.add_edge(2, 5);
     
     int weight_limit = 4;
     
     // Calculate max value if we root at 1 and have max weight 4
     long long max_val = tk.solve(1, weight_limit);
     
     cout << "Max value within weight " << weight_limit << " is: " << max_val << "\n";
     // Output expected: node 1 (w=1, v=5) + node 2 (w=2, v=10) + node 5 (w=1, v=3)
     // Total weight = 4, Total value = 18.
     
     return 0;
   }
*/