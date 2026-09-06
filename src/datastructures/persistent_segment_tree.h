const int N = 3e5 + 5;
const int LOG = 20; 
const int MAX_NODES = N * 40;

struct Node {
  int left, right, v;
  Node(int _left = 0, int _right = 0, int _v = 0) {
    left = _left; 
    right = _right; 
    v = _v;
  }
} st[MAX_NODES];

int cnt = 0;
int ver[N], nver = 0;

void refine(int id) {
  st[id].v = st[st[id].left].v + st[st[id].right].v;
}

int build(int l, int r) {
  if (l == r) {
    cnt++;
    st[cnt] = Node(0, 0, 0);
    return cnt;
  }
  int mid = (l + r) >> 1;
  int cur = ++cnt;
  st[cur].left = build(l, mid);
  st[cur].right = build(mid + 1, r);
  refine(cur);
  return cur;
}

int update(int id, int l, int r, int u, int x) {
  if (l == r) {
    cnt++;
    st[cnt] = Node(0, 0, st[id].v + x); 
    return cnt;
  }
  int mid = (l + r) >> 1;
  int cur = ++cnt;
  if (u <= mid) {
    st[cur].left = update(st[id].left, l, mid, u, x);
    st[cur].right = st[id].right;
  } else {
    st[cur].left = st[id].left;
    st[cur].right = update(st[id].right, mid + 1, r, u, x);
  }
  refine(cur);
  return cur;
}

int get(int id, int l, int r, int u, int v) {
  if (l > v || r < u) return 0;
  if (l >= u && r <= v) return st[id].v;
  int mid = (l + r) >> 1;
  return get(st[id].left, l, mid, u, v) + get(st[id].right, mid + 1, r, u, v);
}

void add_version(int l, int r, int u, int x) {
  nver++;
  ver[nver] = update(ver[nver - 1], l, r, u, x);
}
/*
   USAGE EXAMPLE (Solving Distinct Values in Range - DQUERY):
   
   // 1. Initialize base tree
   cnt = nver = 0;
   ver[0] = build(1, n);
   
   // 2. Build versions tracking the latest occurrence of each value
   for (int i = 1; i <= n; i++) {
     // If value appeared before, remove its previous position
     if (mark[a[i]] > 0) {
       ver[nver] = update(ver[nver], 1, n, mark[a[i]], -1); 
     }
     
     // Add current position
     nver++;
     ver[nver] = update(ver[nver - 1], 1, n, i, 1);
     
     iver[i] = ver[nver]; // iver[i] stores the version for prefix i
     mark[a[i]] = i;
   }
   
   // 3. Query distinct values in [L, R]
   int ans = get(iver[R], 1, n, L, R);
*/