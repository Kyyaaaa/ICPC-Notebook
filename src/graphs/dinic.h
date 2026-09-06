/*
   DINIC'S ALGORITHM (Max Flow & Min Cut)
   Idea:
   - Phase 1 (BFS): Build a "Level Graph". Assign levels to nodes based on 
     shortest path from Source (S). Only edges that go to the next level 
     (level[v] == level[u] + 1) are considered valid.
   - Phase 2 (DFS): Push as much flow as possible along the Level Graph.
   - DEAD-END OPTIMIZATION (ptr array): During DFS, if an edge leads to a 
     dead end (can't push any more flow), we advance the `ptr` so we NEVER 
     visit this useless edge again in the current BFS phase.
   Complexity: 
   - General Graph: O(V^2 * E)
   - Unit Network (e.g., Bipartite Matching): O(E * sqrt(V))
   - Extremely fast in practice, much faster than Edmonds-Karp.
*/
const long long LINF = 1e18;

struct Edge {
  int u, v;
  long long cap, flow;
};

struct Dinic {
  int n;
  vector<Edge> edges;
  vector<vector<int>> adj;
  vector<int> level; // Stores the BFS level of each node
  vector<int> ptr;   // Optimization: keeps track of the next edge to explore in DFS

  Dinic(int _n) {
    n = _n;
    adj.assign(n + 1, vector<int>());
    level.assign(n + 1, -1);
    ptr.assign(n + 1, 0);
  }

  void add_edge(int u, int v, long long cap, bool is_directed = true) {
    adj[u].push_back(edges.size());
    edges.push_back({u, v, cap, 0});
    
    adj[v].push_back(edges.size());
    edges.push_back({v, u, is_directed ? 0 : cap, 0});
  }

  // BFS to build the Level Graph
  bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;
    queue<int> q;
    q.push(s);
    
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int id : adj[u]) {
        int v = edges[id].v;
        // If not visited AND has remaining capacity
        if (level[v] == -1 && edges[id].cap - edges[id].flow > 0) {
          level[v] = level[u] + 1;
          q.push(v);
        }
      }
    }
    // Return true if the sink is reachable
    return level[t] != -1;
  }

  // DFS to push flow along the Level Graph
  long long dfs(int u, int t, long long pushed) {
    if (pushed == 0 || u == t) return pushed;
    
    // Note the reference `&cid = ptr[u]`. This is the core optimization!
    // It prevents checking dead ends multiple times in the same BFS phase.
    for (int& cid = ptr[u]; cid < adj[u].size(); ++cid) {
      int id = adj[u][cid];
      int v = edges[id].v;
      long long tr = edges[id].cap - edges[id].flow; // Remaining capacity
      
      // Only push flow to the exact next level
      if (level[u] + 1 != level[v] || tr == 0) continue;
      
      long long push = dfs(v, t, min(pushed, tr));
      if (push == 0) continue;
      
      edges[id].flow += push;
      edges[id ^ 1].flow -= push;
      return push;
    }
    return 0;
  }

  // Main function to calculate Max Flow
  long long max_flow(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) { // While a path exists
      fill(ptr.begin(), ptr.end(), 0); // Reset the dead-end pointers
      
      // Push as much flow as possible in the current Level Graph
      while (long long pushed = dfs(s, t, LINF)) {
        flow += pushed;
      }
    }
    return flow;
  }

  // Min Cut Extraction: Nodes reachable from Source in the residual graph
  vector<bool> get_min_cut(int s) {
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int id : adj[u]) {
        int v = edges[id].v;
        if (!visited[v] && edges[id].cap - edges[id].flow > 0) {
          visited[v] = true;
          q.push(v);
        }
      }
    }
    return visited; // visited[i] = true means 'i' is in the Source set
  }

  // Tracing: Print edges that carry flow
  void print_flow_paths() {
    cout << "Edges with positive flow:\n";
    for (int i = 0; i < edges.size(); i += 2) { // Check forward edges only
      if (edges[i].flow > 0) {
        cout << edges[i].u << " -> " << edges[i].v 
             << " : " << edges[i].flow << " / " << edges[i].cap << "\n";
      }
    }
  }
};

/*
   USAGE EXAMPLE:
   signed main() {
     int n = 4; // 4 vertices
     Dinic dinic(n);
     
     // add_edge(u, v, capacity)
     dinic.add_edge(1, 2, 3);
     dinic.add_edge(1, 3, 2);
     dinic.add_edge(2, 3, 1);
     dinic.add_edge(2, 4, 2);
     dinic.add_edge(3, 4, 4);
     
     int source = 1, sink = 4;
     cout << "Max Flow: " << dinic.max_flow(source, sink) << "\n"; // Output: 5
     
     // Extract Min Cut
     vector<bool> in_source_set = dinic.get_min_cut(source);
     cout << "Min Cut Edges:\n";
     for (int i = 0; i < dinic.edges.size(); i += 2) {
       int u = dinic.edges[i].u;
       int v = dinic.edges[i].v;
       if (in_source_set[u] && !in_source_set[v]) {
         cout << u << " -> " << v << " (Capacity: " << dinic.edges[i].cap << ")\n";
       }
     }
     
     return 0;
   }
*/