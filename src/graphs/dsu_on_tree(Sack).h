/* =========================================================================
   DSU ON TREE (SACK)
   
   Idea:
   - Efficiently answers OFFLINE queries about subtrees in O(N log N) time.
   - Solves problems like: "For each node U, how many nodes in its subtree 
     have a specific property?"
     
   Algorithm:
   1. Find the "heavy" child (largest subtree) for each node.
   2. DFS traversal:
      - Traverse all "light" children, answer their queries, then CLEAR their data.
      - Traverse the "heavy" child, answer its queries, and KEEP its data.
      - Add the data of the light children and the current node itself.
      - Answer queries for the current node.
      - If the current node is a light child (`keep == false`), CLEAR all its data.
========================================================================= */

const int N = 2e5 + 5;
vector<int> G[N];

// Variables for Euler tour and Heavy-Light logic
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
// Example: Counting frequencies of values
struct GlobalState {
  int freq[N];
  int count_of_freq[N];
  
  void add(int node) {
    // int val = a[node];
    // count_of_freq[++freq[val]]++;
  }
  
  void remove(int node) {
    // int val = a[node];
    // count_of_freq[freq[val]--]--;
  }
  
  // int get_answer(...) { return ...; }
} state;


// 2. Second DFS: DSU on Tree (Sack)
void dsu_on_tree(int u, int par, bool keep) {
  // Step 2.1: Traverse light children and DO NOT keep their data
  for (int v : G[u]) {
    if (v != par && v != big[u]) {
      dsu_on_tree(v, u, false);
    }
  }
  
  // Step 2.2: Traverse the heavy child and KEEP its data
  if (big[u] != 0) {
    dsu_on_tree(big[u], u, true);
  }
  
  // Step 2.3: Add light children's data and the current node's data
  for (int v : G[u]) {
    if (v != par && v != big[u]) {
      for (int i = in[v]; i <= out[v]; i++) {
        state.add(rn[i]);
      }
    }
  }
  state.add(u); // Add the current node itself
  
  // Step 2.4: ANSWER QUERIES for node `u` here
  // for (auto query : queries[u]) {
  //   ans[query.id] = state.get_answer(query.k);
  // }
  
  // Step 2.5: If we are a light child, clear our data so it doesn't affect 
  // other branches of our parent.
  if (!keep) {
    for (int i = in[u]; i <= out[u]; i++) {
      state.remove(rn[i]);
    }
  }
}

/*
   USAGE EXAMPLE:
   signed main() {
     // ... Read graph and queries ...
     // ... Save queries as `queries[u].push_back({k, query_id})` ...
     timer = 0;
     dfs_sz(1, -1);
     // Start DSU on tree from the root, keeping is irrelevant for the root
     dsu_on_tree(1, -1, false);
     // Print answers
     // ...
     return 0;
   }
*/