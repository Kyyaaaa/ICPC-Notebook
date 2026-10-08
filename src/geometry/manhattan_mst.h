// Manhattan MST candidate edges: {weight, u, v}, 0-based point indices.
// Run Kruskal on these edges to obtain the MST. O(N log N) time, O(N) edges.
// Coordinates, transforms and Manhattan distances must fit in ll.
vector<array<ll, 3>> manhattan_mst(vector<Point> ps) {
  int n = (int)ps.size();
  vector<int> id(n);
  iota(id.begin(), id.end(), 0);
  vector<array<ll, 3>> edges;

  for (int k = 0; k < 4; k++) {
    // Sort by x + y. Use __int128 to avoid overflow in this comparison.
    sort(id.begin(), id.end(), [&](int i, int j) {
      return (__int128)ps[i].x + ps[i].y < (__int128)ps[j].x + ps[j].y;
    });

    map<ll, int> sweep;
    for (int i : id) {
      for (auto it = sweep.lower_bound(-ps[i].y); it != sweep.end(); sweep.erase(it++)) {
        int j = it->second;
        Point d = ps[i] - ps[j];
        if (d.y > d.x) break;
        edges.push_back({d.x + d.y, i, j});
      }
      sweep[-ps[i].y] = i;
    }

    for (Point &p : ps) {
      if (k & 1) {
        p.x = -p.x;
      } else {
        swap(p.x, p.y);
      }
    }
  }
  return edges;
}