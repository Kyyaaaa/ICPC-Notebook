// Sort lexicographically by (first, second).
// countingSort(a, k, lo, hi, asc):
//   k = 0: sort by first; k = 1: sort by second.
//   lo, hi: min/max value of component k (inclusive).
//   asc = true: ascending; false: descending.
//   Stable: equal keys preserve their relative order.
//   Time: O(n + R), Memory: O(n + R), R = hi - lo + 1.
// Note: All component values must be within their specified ranges.
// Note: Use counting sort when value ranges are small enough.

using pii = pair<int, int>;

void countingSort(vector<pii>& a, int k, int lo, int hi, bool asc = true) {
  int R = hi - lo + 1;
  vector<int> cnt(R, 0);

  auto key = [&](const pii& x) {
    int v = (k == 0 ? x.first : x.second);
    return asc ? v - lo : hi - v;
  };

  for (auto& x : a)
    cnt[key(x)]++;

  int sum = 0;
  for (int i = 0; i < R; i++) {
    int t = cnt[i];
    cnt[i] = sum;
    sum += t;
  }

  vector<pii> b(a.size());
  for (auto& x : a)
    b[cnt[key(x)]++] = x;

  a.swap(b);
}