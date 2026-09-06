struct Fenwick {
  int n;
  vector<long long> bit;

  Fenwick(int _n) {
    n = _n;
    bit.assign(n + 1, 0);
  }

  void update(int idx, long long delta) {
    for (; idx <= n; idx += idx & -idx) {
      bit[idx] += delta;
    }
  }

  long long query(int idx) {
    long long sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
      sum += bit[idx];
    }
    return sum;
  }

  long long query(int l, int r) {
    if (l > r) return 0;
    return query(r) - query(l - 1);
  }

  // O(log N) Bit Jumping: Finds the FIRST index where prefix sum >= val.
  // Note: This only works if all elements in the BIT are non-negative!
  int lower_bound(long long val) {
    if (val <= 0) return 1;
    int pos = 0;
    long long sum = 0;
    for (int i = __lg(n); i >= 0; i--) {
      int next_pos = pos + (1 << i);
      if (next_pos <= n && sum + bit[next_pos] < val) {
        sum += bit[next_pos];
        pos = next_pos;
      }
    }
    // pos is the largest index such that prefix sum < val
    // So pos + 1 is the first index such that prefix sum >= val
    return pos + 1; 
  }
};