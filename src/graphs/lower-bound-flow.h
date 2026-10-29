/*
   REQUIREMENT:
   - Paste the Dinic template (Edge + Dinic + LINF) BEFORE this template.
   - Original vertices are numbered from 1 to n.
   - Each edge has a lower bound l and upper bound r: l <= flow <= r.
   - Each LowerBoundFlow object supports ONE solve call only.
   - max_flow/min_flow consider non-negative s -> t flow values.

   Complexity:
   - General Graph: O(V^2 * E), up to 2 Max Flow runs.
*/

struct LowerBoundFlow {
  struct OriginalEdge {
    int u, v, id;
    ll lower, upper;
  };

  int n, ss, tt;
  Dinic dinic;
  vector<ll> demand;
  vector<OriginalEdge> original_edges;
  bool used = false, feasible = false;
  ll base_flow = 0;

  LowerBoundFlow(int _n) : n(_n), ss(n + 1), tt(n + 2), dinic(n + 2) {
    demand.assign(n + 3, 0);
  }

  // Add edge u -> v with l <= flow <= r.
  // Return an ID (0-based) for get_flow().
  int add_edge(int u, int v, ll l, ll r) {
    assert(!used && 1 <= u && u <= n && 1 <= v && v <= n);
    assert(0 <= l && l <= r);

    int id = dinic.edges.size();
    dinic.add_edge(u, v, r - l);

    demand[u] -= l;
    demand[v] += l;

    original_edges.push_back({u, v, id, l, r});
    return (int)original_edges.size() - 1;
  }

  // Actual flow on an original edge, INCLUDING its lower bound.
  // Call only after a successful solve.
  ll get_flow(int edge_id) const {
    assert(feasible && 0 <= edge_id && edge_id < (int)original_edges.size());
    const auto &e = original_edges[edge_id];
    return e.lower + dinic.edges[e.id].flow;
  }

  // Disable both directions of an auxiliary edge in the residual graph.
  void disable_edge(int id) {
    dinic.edges[id].cap = dinic.edges[id].flow = 0;
    dinic.edges[id ^ 1].cap = dinic.edges[id ^ 1].flow = 0;
  }

  // Internal: build and check feasible circulation.
  // If s,t are supplied, temporarily add t -> s with capacity LINF.
  bool prepare(int s = -1, int t = -1) {
    assert(!used);
    used = true;

    int back_edge = -1;
    if (s != -1) {
      assert(1 <= s && s <= n && 1 <= t && t <= n && s != t);
      back_edge = dinic.edges.size();
      dinic.add_edge(t, s, LINF);
    }

    vector<int> auxiliary_edges;
    ll required = 0;

    for (int v = 1; v <= n; v++) {
      if (demand[v] > 0) {
        int id = dinic.edges.size();
        dinic.add_edge(ss, v, demand[v]);
        auxiliary_edges.push_back(id);
        required += demand[v];
      } else if (demand[v] < 0) {
        int id = dinic.edges.size();
        dinic.add_edge(v, tt, -demand[v]);
        auxiliary_edges.push_back(id);
      }
    }

    if (dinic.max_flow(ss, tt) != required) return false;

    feasible = true;
    if (back_edge != -1) {
      base_flow = dinic.edges[back_edge].flow;
      disable_edge(back_edge);
    }

    for (int id : auxiliary_edges) disable_edge(id);
    return true;
  }

  // Max flow with a limit, using the EXISTING Dinic bfs() and dfs().
  ll max_flow_limited(int s, int t, ll limit) {
    ll ans = 0;
    while (ans < limit && dinic.bfs(s, t)) {
      fill(dinic.ptr.begin(), dinic.ptr.end(), 0);
      while (ans < limit) {
        ll pushed = dinic.dfs(s, t, min(LINF, limit - ans));
        if (!pushed) break;
        ans += pushed;
      }
    }
    return ans;
  }

  // No source/sink: conservation at ALL vertices.
  bool feasible_circulation() {
    return prepare();
  }

  // Check existence of a non-negative s -> t flow.
  bool feasible_flow(int s, int t) {
    return prepare(s, t);
  }

  // Return {exists, maximum flow value}.
  pair<bool, ll> max_flow(int s, int t) {
    if (!prepare(s, t)) return {false, 0};
    return {true, base_flow + dinic.max_flow(s, t)};
  }

  // Return {exists, minimum NON-NEGATIVE flow value}.
  pair<bool, ll> min_flow(int s, int t) {
    if (!prepare(s, t)) return {false, 0};
    return {true, base_flow - max_flow_limited(t, s, base_flow)};
  }

  // Tracing: print all original edges carrying positive flow.
  void print_flow_paths() const {
    assert(feasible);
    cout << "Edges with positive flow:\n";
    for (int i = 0; i < (int)original_edges.size(); i++) {
      ll f = get_flow(i);
      if (f > 0) {
        const auto &e = original_edges[i];
        cout << e.u << " -> " << e.v << " : "
             << f << " / [" << e.lower << ", " << e.upper << "]\n";
      }
    }
  }
};
/*
   USAGE EXAMPLES:

   // 1. Maximum s -> t flow:
   LowerBoundFlow f(n);
   int id = f.add_edge(1, 2, 2, 5);
   f.add_edge(2, 3, 1, 7);

   auto [ok, ans] = f.max_flow(1, 3);
   if (!ok) cout << -1 << '\n';
   else {
     cout << "Max Flow: " << ans << '\n';
     cout << "Flow on edge id: " << f.get_flow(id) << '\n';
     f.print_flow_paths();
   }

   // 2. Minimum NON-NEGATIVE s -> t flow (use a NEW object):
   LowerBoundFlow g(n);
   // g.add_edge(u, v, l, r);
   auto [ok2, min_ans] = g.min_flow(s, t);

   // 3. Feasible circulation (no designated source/sink):
   LowerBoundFlow h(n);
   // h.add_edge(u, v, l, r);
   bool possible = h.feasible_circulation();

   // 4. Just check whether a non-negative s -> t flow exists:
   LowerBoundFlow k(n);
   // k.add_edge(u, v, l, r);
   bool possible_st = k.feasible_flow(s, t);

   NOTES:
   - Do NOT call two solve methods on the same object.
   - Do NOT add t -> s yourself when using max_flow/min_flow/feasible_flow.
   - get_flow(id) is available only after a successful solve.
*/
