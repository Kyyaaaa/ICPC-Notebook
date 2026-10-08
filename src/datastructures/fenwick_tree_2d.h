struct Fenwick2D {
  int n, m;
  vector<vector<long long>> bit1, bit2, bit3, bit4;

  Fenwick2D(int _n, int _m) {
    n = _n;
    m = _m;
    bit1.assign(n + 2, vector<long long>(m + 2, 0));
    bit2.assign(n + 2, vector<long long>(m + 2, 0));
    bit3.assign(n + 2, vector<long long>(m + 2, 0));
    bit4.assign(n + 2, vector<long long>(m + 2, 0));
  }

  void add(int x, int y, long long val) {
    for (int i = x; i <= n; i += i & -i) {
      for (int j = y; j <= m; j += j & -j) {
        bit1[i][j] += val;
        bit2[i][j] += val * x;
        bit3[i][j] += val * y;
        bit4[i][j] += val * x * y;
      }
    }
  }

  void update(int x1, int y1, int x2, int y2, long long val) {
    add(x1, y1, val);
    add(x2 + 1, y1, -val);
    add(x1, y2 + 1, -val);
    add(x2 + 1, y2 + 1, val);
  }

  // Internal Prefix Query: gets the sum of submatrix from (1, 1) to (x, y)
  long long query_prefix(int x, int y) {
    long long res = 0;
    for (int i = x; i > 0; i -= i & -i) {
      for (int j = y; j > 0; j -= j & -j) {
        res += bit1[i][j] * (x + 1) * (y + 1)
             - bit2[i][j] * (y + 1)
             - bit3[i][j] * (x + 1)
             + bit4[i][j];
      }
    }
    return res;
  }

  // Range Query: gets the sum of submatrix from (x1, y1) to (x2, y2)
  long long query(int x1, int y1, int x2, int y2) {
    return query_prefix(x2, y2) 
         - query_prefix(x1 - 1, y2) 
         - query_prefix(x2, y1 - 1) 
         + query_prefix(x1 - 1, y1 - 1);
  }
};