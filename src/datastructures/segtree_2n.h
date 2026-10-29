#define ll long long
struct SegTree {
  // Iterative Segment Tree (2*n) with Lazy Propagation.
  // Supports Range Add and Range Sum.
  //
  // INDEXING:
  // - Array indices: 0-based [0, n-1].
  // - Tree node indices: 1-based, root = 1.
  // - Leaf of a[i]: st[n + i].
  // - All range operations use [l, r) (0-based).
  // - Point operations use p in [0, n-1].
  //
  // Time: O(n) build, O(log n) per operation.
  // Space: O(n).

  int n;
  vector<ll> st, lazy;
  vector<int> len;

  // Initialize an array of n zeros (0-based).
  SegTree(int _n = 0) {
    n = _n;
    st.assign(2 * n, 0);
    lazy.assign(n, 0);
    len.assign(2 * n, 0);

    // Leaf st[n + i] represents a[i].
    for (int i = n; i < 2 * n; i++)
      len[i] = 1;

    // Initialize segment lengths bottom-up.
    for (int p = n - 1; p > 0; p--)
      len[p] = len[p << 1] + len[p << 1 | 1];
  }

  // Build from a 0-based array a[0...n-1].
  SegTree(const vector<ll>& a) : SegTree((int)a.size()) {
    // Copy a[i] to leaf st[n + i].
    for (int i = 0; i < n; i++)
      st[n + i] = a[i];

    // Build the tree bottom-up.
    for (int p = n - 1; p > 0; p--)
      st[p] = st[p << 1] + st[p << 1 | 1];
  }

  // Apply addition x to the subtree rooted at p.
  // p is a tree node index, not an array index.
  void apply(int p, ll x) {
    st[p] += x * len[p];
    if (p < n) lazy[p] += x;
  }

  // Push pending updates from node p to its children.
  void push(int p) {
    if (lazy[p] == 0) return;

    apply(p << 1, lazy[p]);
    apply(p << 1 | 1, lazy[p]);
    lazy[p] = 0;
  }

  // Recalculate node p from its children.
  void pull(int p) {
    st[p] = st[p << 1] + st[p << 1 | 1]
          + lazy[p] * len[p];
  }

  // Push pending updates from the root to node p.
  // p is a tree node index.
  void pushPath(int p) {
    int h = 31 - __builtin_clz((unsigned)p);

    for (int s = h; s > 0; s--)
      push(p >> s);
  }

  // Recalculate all ancestors of node p.
  void pullPath(int p) {
    while (p > 1) {
      p >>= 1;
      pull(p);
    }
  }

  // Add x to a[l...r-1].
  // 0-based, half-open interval [l, r).
  // Example: rangeAdd(0, 3, x) updates a[0], a[1], a[2].
  void rangeAdd(int l, int r, ll x) {
    if (l >= r) return;

    // Convert array indices to tree leaf indices.
    l += n;
    r += n;

    int l0 = l, r0 = r - 1;

    // Propagate pending updates at both boundaries.
    pushPath(l0);
    pushPath(r0);

    // Apply updates to the segment decomposition.
    while (l < r) {
      if (l & 1) apply(l++, x);
      if (r & 1) apply(--r, x);

      l >>= 1;
      r >>= 1;
    }

    // Recalculate affected ancestors.
    pullPath(l0);
    pullPath(r0);
  }

  // Set a[p] = x.
  // p is a 0-based array index: 0 <= p < n.
  void pointSet(int p, ll x) {
    // Convert array index to tree leaf index.
    p += n;

    pushPath(p);
    st[p] = x;
    pullPath(p);
  }

  // Add x to a[p].
  // p is a 0-based array index: 0 <= p < n.
  void pointAdd(int p, ll x) {
    rangeAdd(p, p + 1, x);
  }

  // Return sum(a[l...r-1]).
  // 0-based, half-open interval [l, r).
  // Example: rangeSum(1, 4) = a[1] + a[2] + a[3].
  ll rangeSum(int l, int r) {
    if (l >= r) return 0;

    // Convert array indices to tree leaf indices.
    l += n;
    r += n;

    pushPath(l);
    pushPath(r - 1);

    ll res = 0;

    while (l < r) {
      if (l & 1) res += st[l++];
      if (r & 1) res += st[--r];

      l >>= 1;
      r >>= 1;
    }

    return res;
  }

  // Return a[p].
  // p is a 0-based array index: 0 <= p < n.
  ll pointGet(int p) {
    // Convert array index to tree leaf index.
    p += n;

    pushPath(p);
    return st[p];
  }
};
