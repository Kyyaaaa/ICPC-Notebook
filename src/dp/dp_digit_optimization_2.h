/*   
  DIGIT DP OPTIMIZATION 2: ONE-TIME MEMSET FOR MULTI-TEST CASES
*/

const int mod = 1e9 + 7;

string R;
int dp[51][51][51][51];

int calc(int pos, int c3, int c6, int c9, int fr) {
  if (pos == -1) {
    return (c3 > 0 && c3 == c6 && c6 == c9);
  } 
  auto &res = dp[pos][c3][c6][c9];
  if (fr && res != -1) return res;
  ll val = 0;
  int lim_r = (fr ? 9 : R[pos] - '0');
  for (int i = 0; i <= lim_r; i++) {
    val += calc(pos - 1, c3 + (i == 3), c6 + (i == 6), c9 + (i == 9), fr | (i < lim_r));
    if (val >= mod) val -= mod;
  }
  return (fr ? res = val : val);
}

int solve(string x) {
  reverse(x.begin(), x.end());
  R = x;
  return calc((int)R.size() - 1, 0, 0, 0, 0);
}

bool check(string x) {
  int c3 = 0, c6 = 0, c9 = 0;
  for (auto i : x) {
    c3 += (i == '3');
    c6 += (i == '6');
    c9 += (i == '9');
  }
  return (c3 > 0 && c3 == c6 && c6 == c9);
}

/*
// Usage in main():
// memset(dp, -1, sizeof dp); // Call this ONLY ONCE
// while (test--) {
//   string a, b; cin >> a >> b;
//   cout << (1LL * solve(b) - solve(a) + check(a) + mod) % mod << "\n";
// }
*/