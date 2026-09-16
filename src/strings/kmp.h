vector<int> compute_pi(const string& p) {
  int m = p.length();
  vector<int> pi(m, 0);
  for (int i = 1, j = 0; i < m; i++) {
    while (j > 0 && p[i] != p[j]) {
      j = pi[j - 1];
    }
    if (p[i] == p[j]) {
      j++;
    }
    pi[i] = j;
  }
  return pi;
}