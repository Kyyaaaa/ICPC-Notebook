const int N = 2e5 + 5;
vector<int> G[N];
int sz[N], son[N], depth[N], pre[N];
int in[N], cnt, top[N];

void dfs1(int u, int par) {
  sz[u] = 1;
  son[u] = 0;
  pre[u] = par;
  for (int v : G[u]) {
    if (v != par) {
      depth[v] = depth[u] + 1;
      dfs1(v, u);
      sz[u] += sz[v];
      if (sz[v] > sz[son[u]]) {
        son[u] = v;
      }
    }
  }
}

void rebuild(int u, int topp) {
  in[u] = ++cnt;
  top[u] = topp;
  if (son[u]) {
    rebuild(son[u], topp);
  }
  for (int v : G[u]) {
    if (v != pre[u] && v != son[u]) {
      rebuild(v, v);
    }
  }
}

int query_path(int x, int y) {
  int res = 0;
  while (top[x] != top[y]) {
    if (depth[top[x]] < depth[top[y]]) {
      swap(x, y);
    }
    res = max(res, get(1, 1, n, in[top[x]], in[x])); 
    x = pre[top[x]];
  }
  if (depth[x] > depth[y]) {
    swap(x, y);
  }
  res = max(res, get(1, 1, n, in[x], in[y]));
  return res;
}

void update_path(int x, int y, int val) {
  while (top[x] != top[y]) {
    if (depth[top[x]] < depth[top[y]]) {
      swap(x, y);
    }
    x = pre[top[x]];
  }
  if (depth[x] > depth[y]) {
    swap(x, y);
  }
}
/*
   USAGE EXAMPLE:
   dfs1(1, 1);
   cnt = 0;
   rebuild(1, 1);
   // for(int i = 1; i <= n; i++) update(1, 1, n, in[i], a[i]);
*/