/*
   Idea:
   - Repeatedly find an augmenting path from Source (S) to Sink (T) using BFS.
   - Bottleneck of the path is the minimum remaining capacity among its edges.
   - Push the bottleneck flow through the path, and ADD it to the reverse 
     edges (to allow "undoing" flow later).
   - Terminates when no more augmenting paths can be found.
   Complexity: O(V * E^2). Safe for V <= 1000, E <= 5000.
*/
const long long LINF = 1e18;

struct Edge {
  int u, v;
  long long cap, flow;
};

struct MaxFlow {
  int n;
  vector<Edge> edges;
  vector<vector<int>> adj;
  
  MaxFlow(int _n) {
    n = _n;
    adj.assign(n + 1, vector<int>());
  }

  // Add a directed edge from u to v with capacity `cap`.
  // If it's an undirected edge, set `is_directed = false`.
  void add_edge(int u, int v, long long cap, bool is_directed = true) {
    adj[u].push_back(edges.size());
    edges.push_back({u, v, cap, 0});       // Forward edge
    
    adj[v].push_back(edges.size());
    edges.push_back({v, u, is_directed ? 0 : cap, 0}); // Backward (residual) edge
  }

  // Find max flow from s to t
  long long max_flow(int s, int t) {
    long long total_flow = 0;
    vector<int> parent_edge(n + 1);

    while (true) {
      // 1. BFS to find an augmenting path
      fill(parent_edge.begin(), parent_edge.end(), -1);
      queue<int> q;
      q.push(s);
      parent_edge[s] = -2; // Mark source as visited

      while (!q.empty() && parent_edge[t] == -1) {
        int u = q.front();
        q.pop();

        for (int id : adj[u]) {
          int v = edges[id].v;
          // If v is not visited AND the edge has remaining capacity
          if (parent_edge[v] == -1 && edges[id].cap - edges[id].flow > 0) {
            parent_edge[v] = id; // Store the edge ID used to reach v
            q.push(v);
          }
        }
      }

      // 2. If Sink (t) is not reached, no more paths exist -> break
      if (parent_edge[t] == -1) break;

      // 3. Find the bottleneck capacity along the path found by BFS
      long long push = LINF;
      int cur = t;
      while (cur != s) {
        int id = parent_edge[cur];
        push = min(push, edges[id].cap - edges[id].flow);
        cur = edges[id].u;
      }

      // 4. Augment the flow along the path and update residual edges
      cur = t;
      while (cur != s) {
        int id = parent_edge[cur];
        edges[id].flow += push;           // Add flow to forward edge
        edges[id ^ 1].flow -= push;       // Subtract flow from backward edge (undo capability)
        cur = edges[id].u;
      }

      total_flow += push;
    }
    return total_flow;
  }

  // Get Min Cut: Returns a boolean array where visited[i] = true means 
  // vertex `i` belongs to the Source's component (Set S).
  vector<bool> get_min_cut(int s) {
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;

    // BFS one last time on edges that are NOT fully saturated
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
    return visited;
  }

  // Tracing: Print all edges that actually carry water in the final state
  void print_flow_paths() {
    cout << "Edges with positive flow:\n";
    // We step by 2 to only check the original forward edges (0, 2, 4...)
    for (int i = 0; i < edges.size(); i += 2) {
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
     MaxFlow mf(n);
    
     // add_edge(u, v, capacity)
     mf.add_edge(1, 2, 3);
     mf.add_edge(1, 3, 2);
     mf.add_edge(2, 3, 1);
     mf.add_edge(2, 4, 2);
     mf.add_edge(3, 4, 4);
     
     int source = 1, sink = 4;
     cout << "Max Flow: " << mf.max_flow(source, sink) << "\n";
     
     // Print flow network details
     mf.print_flow_paths();
     
     // Find the Min Cut edges
     vector<bool> in_source_set = mf.get_min_cut(source);
     cout << "Min Cut Edges:\n";
     for (int i = 0; i < mf.edges.size(); i += 2) {
       int u = mf.edges[i].u;
       int v = mf.edges[i].v;
       // An edge is in the Min Cut if it goes from the Source Set to the Sink Set
       if (in_source_set[u] && !in_source_set[v]) {
         cout << u << " -> " << v << " (Capacity cut: " << mf.edges[i].cap << ")\n";
       }
     }
     
     return 0;
   }
*/