struct node {
  node *l, *r;
  int v;

  node() {
    l = r = nullptr;
    v = 0;
  }
};

const int MAX_VAL = 300005;
node *t[MAX_VAL];

void refine(node *cur) {
  cur->v = cur->l->v + cur->r->v;
}

void build(node *cur, int l, int r) {
  cur->l = new node();
  cur->r = new node();
  if (l == r) {
    return;
  }
  int mid = (l + r) / 2;
  build(cur->l, l, mid);
  build(cur->r, mid + 1, r);
  refine(cur);
}

void update(node *cur, int l, int r, int u, int x) {
  if (l == r) {
    cur->v += x;
    return;
  }
  int mid = (l + r) / 2;
  if (u <= mid) {
    node *old = cur->l;
    cur->l = new node();
    cur->l->l = old->l;
    cur->l->r = old->r;
    cur->l->v = old->v;
    update(cur->l, l, mid, u, x);
  } else {
    node *old = cur->r;
    cur->r = new node();
    cur->r->l = old->l;
    cur->r->r = old->r;
    cur->r->v = old->v;
    update(cur->r, mid + 1, r, u, x);
  }
  refine(cur);
}

node* update_version(node *prev_root, int l, int r, int pos, int val) {
  node *cur = new node();
  cur->l = prev_root->l;
  cur->r = prev_root->r;
  cur->v = prev_root->v;
  update(cur, l, r, pos, val);
  return cur;
}

int query(node *cur, int l, int r, int u, int v) {
  if (l > v || r < u) return 0;
  if (l >= u && r <= v) return cur->v;
  int mid = (l + r) / 2;
  return query(cur->l, l, mid, u, v) + query(cur->r, mid + 1, r, u, v);
}

int query_diff(node *rootR, node *rootL, int l, int r, int u, int v) {
  if (l > v || r < u) return 0;
  if (l >= u && r <= v) return rootR->v - rootL->v;
  int mid = (l + r) / 2;
  return query_diff(rootR->l, rootL->l, l, mid, u, v) 
       + query_diff(rootR->r, rootL->r, mid + 1, r, u, v);
}