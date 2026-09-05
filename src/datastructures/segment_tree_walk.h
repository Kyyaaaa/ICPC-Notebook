/* Find first element in range [u, v] have value >= val */
int findPos(int id, int l, int r, int u, int v, ll val) {
  if(l > v || r < u) return n + 1;
  if(st[id] < val) return n + 1; 
  if(l == r) return l;
  push(id);
  int mid = (l + r) / 2;
  int res = findPos(id * 2, l, mid, u, v, val);
  if(res == n + 1) {
    res = findPos(id * 2 + 1, mid + 1, r, u, v, val);
  }
  return res;
}