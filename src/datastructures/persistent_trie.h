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

/*
   USAGE EXAMPLE:
   
   // 1. Build prefix versions
   init_trie();
   for (int i = 1; i <= n; i++) {
     root[i] = add_version(root[i - 1], a[i]);
   }
*/