const int LOG_MAX = 29;
const int N = 1e5 + 5;
const int MAX_NODES = N * 32;

int t[MAX_NODES][2];
int num[MAX_NODES];
int root[N];
int cnt = 0;

void init_trie() {
  for (int i = 0; i <= cnt; i++) {
    t[i][0] = t[i][1] = num[i] = 0;
  }
  cnt = 0;
  root[0] = 0;
}

int newnode() {
  cnt++;
  t[cnt][0] = t[cnt][1] = num[cnt] = 0;
  return cnt;
}

int add_version(int prev_root, int val) {
  int cur_root = newnode();
  int cur = cur_root;
  int old = prev_root;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    t[cur][0] = t[old][0];
    t[cur][1] = t[old][1];
    t[cur][x] = newnode();
    cur = t[cur][x];
    old = t[old][x];
    t[cur][0] = t[old][0];
    t[cur][1] = t[old][1];
    num[cur] = num[old] + 1;
  }
  return cur_root;
}

// 4. Query Maximum XOR with 'val' using combinations of versions.
// Example: Query on a tree path from u to v with LCA p, parent of p is pp.
// The effective count of a node is: num[x] + num[y] - num[p] - num[pp].
int query_max_xor(int rootX, int rootY, int rootP, int rootPP, int val) {
  int res = 0;
  int x = rootX, y = rootY, p = rootP, pp = rootPP;
  for (int i = LOG_MAX; i >= 0; i--) {
    int v = (val >> i) & 1;
    int count_opposite = num[t[x][v ^ 1]] + num[t[y][v ^ 1]] 
                       - num[t[p][v ^ 1]] - num[t[pp][v ^ 1]];
    if (count_opposite > 0) {
      res |= (1 << i);
      x = t[x][v ^ 1];
      y = t[y][v ^ 1];
      p = t[p][v ^ 1];
      pp = t[pp][v ^ 1];
    } else {
      x = t[x][v];
      y = t[y][v];
      p = t[p][v];
      pp = t[pp][v];
    }
  }
  return res;
}

/*
   USAGE EXAMPLE (Max XOR in an array from L to R):
   
   // 1. Build prefix versions
   init_trie();
   for (int i = 1; i <= n; i++) {
     root[i] = add_version(root[i - 1], a[i]);
   }
   
   // 2. Query Max XOR with 'val' in subarray [L, R]
   // For arrays, the effective tree is root[R] - root[L-1].
   // You can use a modified query function that only takes 2 roots:
   // count_opposite = num[t[rootR][v ^ 1]] - num[t[rootL_minus_1][v ^ 1]];
*/