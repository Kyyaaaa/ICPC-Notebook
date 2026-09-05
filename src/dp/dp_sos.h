const int LOG = 20;
int dp[1 << LOG];

// 1. Sum over Submasks: dp[mask] = \sum_{sub \subseteq mask} A[sub]
// (sub is formed by turning OFF some bits of mask)
void sos_submask(int n, int a[]) {
  for (int mask = 0; mask < (1 << n); mask++) {
    dp[mask] = a[mask];
  }
  for (int i = 0; i < n; i++) {
    for (int mask = 0; mask < (1 << n); mask++) {
      if ((mask >> i) & 1) {
        dp[mask] += dp[mask ^ (1 << i)]; 
      }
    }
  }
}

// 2. Sum over Supermasks: dp[mask] = \sum_{mask \subseteq sup} A[sup]
// (sup is formed by turning ON some bits of mask)
void sos_supermask(int n, int a[]) {
  for (int mask = 0; mask < (1 << n); mask++) {
    dp[mask] = a[mask];
  }
  for (int i = 0; i < n; i++) {
    for (int mask = 0; mask < (1 << n); mask++) {
      if (!((mask >> i) & 1)) {
        dp[mask] += dp[mask ^ (1 << i)];
      }
    }
  }
}
/*
  COMMON USAGE PATTERNS & TRICKS:
   1. Find pairs (u, v) such that u & v = 0:
   2. Number of elements `x` such that (x & mask) == x:
   3. Number of elements `x` such that (x & mask) == mask:
   4. Finding two elements x, y with maximum (x | y):
*/