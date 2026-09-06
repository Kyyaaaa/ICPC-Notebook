const int LOG_MAX = 29;
const int MAX_NODES = 3e5 * 32;

struct Node {
  int c[2];
  int cnt;

  Node() {
    c[0] = c[1] = 0;
    cnt = 0;
  }
};

Node trie[MAX_NODES];
int node_cnt = 0;

void init_trie() {
  for (int i = 0; i <= node_cnt; i++) {
    trie[i] = Node();
  }
  node_cnt = 0;
}

void insert(int val) {
  int cur = 0;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    if (trie[cur].c[x] == 0) {
      trie[cur].c[x] = ++node_cnt;
    }
    cur = trie[cur].c[x];
    trie[cur].cnt++;
  }
}

bool search(int val) {
  int cur = 0;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    cur = trie[cur].c[x];
    if (cur == 0 || trie[cur].cnt == 0) {
      return false;
    }
  }
  return trie[cur].cnt > 0;
}

int count_occurences(int val) {
  int cur = 0;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    cur = trie[cur].c[x];
    if (cur == 0 || trie[cur].cnt == 0) {
      return 0;
    }
  }
  return trie[cur].cnt;
}

void erase(int val) {
  if (!search(val)) return;
  int cur = 0;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    cur = trie[cur].c[x];
    trie[cur].cnt--;
  }
}

void update(int old_val, int new_val) {
  if (!search(old_val)) return;
  erase(old_val);
  insert(new_val);
}