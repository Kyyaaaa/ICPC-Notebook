/* =========================================================================
   MIN COST MAX FLOW (Dijkstra with Johnson's Potentials)
   
   Idea:
   - Uses Dijkstra instead of SPFA to find the cheapest augmenting path.
   - Maintains a potential function `pi` for each node to handle negative 
     weight residual edges.
   - Edge weights are reweighted: w'(u, v) = w(u, v) + pi[u] - pi[v] >= 0.
   - Complexity: O(F * E log V) where F is the flow. Fast and safe!
========================================================================= */
const long long LINF = 1e18;

struct MCMFEdge {
  int u, v;
  long long cap, flow, cost;
};

struct MinCostMaxFlow {
  int n;
  vector<MCMFEdge> edges;
  vector<vector<int>> adj;
  vector<long long> pi, dist;
  vector<int> parent_edge;

  MinCostMaxFlow(int _n) {
    n = _n;
    adj.assign(n + 1, vector<int>());
    pi.assign(n + 1, 0);
    dist.assign(n + 1, 0);
    parent_edge.assign(n + 1, -1);
  }

  void add_edge(int u, int v, long long cap, long long cost, bool is_directed = true) {
    adj[u].push_back(edges.size());
    edges.push_back({u, v, cap, 0, cost});
    
    adj[v].push_back(edges.size());
    edges.push_back({v, u, is_directed ? 0 : cap, 0, -cost});
  }

  // Run SPFA once to set initial potentials. 
  // ONLY UNCOMMENT inside mcmf() if the original graph has NEGATIVE edge costs.
  void set_pi(int s) {
    fill(pi.begin(), pi.end(), LINF);
    vector<bool> in_queue(n + 1, false);
    queue<int> q;
    
    pi[s] = 0;
    q.push(s);
    in_queue[s] = true;

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      in_queue[u] = false;

      for (int id : adj[u]) {
        int v = edges[id].v;
        if (edges[id].cap - edges[id].flow > 0 && pi[v] > pi[u] + edges[id].cost) {
          pi[v] = pi[u] + edges[id].cost;
          if (!in_queue[v]) {
            q.push(v);
            in_queue[v] = true;
          }
        }
      }
    }
  }

  // Dijkstra using reduced costs
  bool dijkstra(int s, int t) {
    fill(dist.begin(), dist.end(), LINF);
    fill(parent_edge.begin(), parent_edge.end(), -1);
    
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    
    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
      long long d = pq.top().first;
      int u = pq.top().second;
      pq.pop();

      if (d > dist[u]) continue;

      for (int id : adj[u]) {
        int v = edges[id].v;
        long long remain_cap = edges[id].cap - edges[id].flow;
        
        if (remain_cap > 0) {
          long long reduced_cost = edges[id].cost + pi[u] - pi[v];
          if (dist[v] > dist[u] + reduced_cost) {
            dist[v] = dist[u] + reduced_cost;
            parent_edge[v] = id;
            pq.push({dist[v], v});
          }
        }
      }
    }
    return dist[t] != LINF;
  }

  pair<long long, long long> mcmf(int s, int t, long long max_required_flow = LINF) {
    long long total_flow = 0;
    long long total_cost = 0;

    // Uncomment this line ONLY IF the initial graph contains negative costs
    // set_pi(s); 

    while (total_flow < max_required_flow && dijkstra(s, t)) {
      long long push = max_required_flow - total_flow;
      int cur = t;
      while (cur != s) {
        int id = parent_edge[cur];
        push = min(push, edges[id].cap - edges[id].flow);
        cur = edges[id].u;
      }

      cur = t;
      while (cur != s) {
        int id = parent_edge[cur];
        edges[id].flow += push;
        edges[id ^ 1].flow -= push;
        total_cost += push * edges[id].cost;
        cur = edges[id].u;
      }

      total_flow += push;

      // Update potentials for the next iteration
      for (int i = 1; i <= n; i++) {
        if (dist[i] != LINF) {
          pi[i] += dist[i];
        }
      }
    }

    return {total_flow, total_cost};
  }

  // -----------------------------------------------------------------
  // PATH TRACING: Prints all edges that are actively carrying flow
  // -----------------------------------------------------------------
  void print_flow_paths() {
    cout << "Edges with positive flow (Min Cost Max Flow):\n";
    // Iterate by 2 to check only the original forward edges
    for (int i = 0; i < edges.size(); i += 2) {
      if (edges[i].flow > 0) {
        cout << edges[i].u << " -> " << edges[i].v 
             << " | Flow: " << edges[i].flow << " / " << edges[i].cap 
             << " | Unit Cost: " << edges[i].cost 
             << " | Total Edge Cost: " << edges[i].flow * edges[i].cost << "\n";
      }
    }
  }
};