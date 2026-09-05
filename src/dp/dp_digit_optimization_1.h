/*
  DIGIT DP OPTIMIZATION 1: REMOVING THE `is_less` (fr) DIMENSION
*/

vector<int> R;
pair<ll, ll> dp[20];

pair<ll, ll> calc(int pos, int fr) {
  if (pos == -1) return make_pair(0, 1);
  auto &res = dp[pos];
  if (fr && res != make_pair(-1LL, -1LL)) return res;
  int lim_r = (fr ? 9 : R[pos]);
  pair<ll, ll> val = make_pair(0, 0);
  for (int i = 0; i <= lim_r; i++) {
    pair<ll, ll> nxt = calc(pos - 1, fr | (i < lim_r));
    val.first += nxt.first + i * nxt.second;
    val.second += nxt.second;
  }
  return (fr ? res = val : val);
}