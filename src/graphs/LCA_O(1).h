const int N = 1e5 + 5;
const int LOG = 18;

vector<int> G[N];
int tin[N], cnt, rn[N];
int euler[2 * N], ct, first[N];
int depth[N];
int rmq[LOG][2 * N];

void dfs(int u, int p = 0) {
  tin[u] = ++cnt;
  rn[cnt] = u;
  euler[++ct] = tin[u];
  first[tin[u]] = ct; 
  for (int v : G[u]) {
    if (v == p) continue;
    depth[v] = depth[u] + 1;
    dfs(v, u);
    euler[++ct] = tin[u];
  }
}

void build_lca() {
  for (int i = 1; i <= ct; i++) {
    rmq[0][i] = euler[i];
  }
  for (int j = 1; j < LOG; j++) {
    for (int i = 1; i <= ct - (1 << (j - 1)); i++) {
      rmq[j][i] = min(rmq[j - 1][i], rmq[j - 1][i + (1 << (j - 1))]);
    }
  }
}

int lca(int x, int y) {
  x = first[tin[x]];
  y = first[tin[y]];
  if (x > y) swap(x, y);
  int k = 31 - __builtin_clz(y - x + 1);
  int min_tin = min(rmq[k][x], rmq[k][y - (1 << k) + 1]);
  return rn[min_tin];
}