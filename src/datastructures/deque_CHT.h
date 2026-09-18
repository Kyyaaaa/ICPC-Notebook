/*
Trick for MINIMUM QUERIES:
   - Add lines as: add(-k, -m)
   - Query as: -query_increasing_x(x) or -query_binary_search(x)
*/
struct Line {
  long long k, m;
  long long get(long long x) { return k * x + m; }
};

struct DequeCHT {
  deque<Line> dq;

  long long floor_div(long long a, long long b) {
    return a / b - ((a ^ b) < 0 && a % b);
  }

  long long intersect(Line a, Line b) {
    return floor_div(a.m - b.m, b.k - a.k);
  }

  // Adds a line y = k*x + m.
  // IMPORTANT: Slopes (k) must be inserted in non-decreasing order!
  void add(long long k, long long m) {
    Line L = {k, m};
    
    if (!dq.empty() && dq.back().k == k) {
      if (m <= dq.back().m) return;
      dq.pop_back();
    }
    
    while (dq.size() >= 2) {
      Line l1 = dq[dq.size() - 2];
      Line l2 = dq.back();
      if (intersect(l1, l2) >= intersect(l2, L)) {
        dq.pop_back();
      } else {
        break;
      }
    }
    dq.push_back(L);
  }

  // 1. O(1) Query: ONLY use when query values 'x' are monotonically INCREASING!
  long long query_increasing_x(long long x) {
    assert(!dq.empty());
    while (dq.size() >= 2 && dq[0].get(x) <= dq[1].get(x)) {
      dq.pop_front();
    }
    return dq[0].get(x);
  }

  // 2. O(log N) Query: Use when 'x' values are RANDOM / NOT SORTED
  long long query_binary_search(long long x) {
    assert(!dq.empty());
    int l = 0, r = dq.size() - 2;
    int ans = dq.size() - 1;

    while (l <= r) {
      int mid = l + (r - l) / 2;
      if (intersect(dq[mid], dq[mid + 1]) < x) {
        l = mid + 1;
      } else {
        ans = mid;
        r = mid - 1;
      }
    }
    return dq[ans].get(x);
  }
};