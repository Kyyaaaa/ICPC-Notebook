const int LOG_MAX = 29;

struct Node {
  Node* c[2];
  int cnt;

  Node() {
    c[0] = c[1] = nullptr;
    cnt = 0;
  }
};

void insert(Node* root, int val) {
  Node* cur = root;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    if (cur->c[x] == nullptr) {
      cur->c[x] = new Node();
    }
    cur = cur->c[x];
    cur->cnt++;
  }
}

bool search(Node* root, int val) {
  Node* cur = root;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    if (cur->c[x] == nullptr || cur->c[x]->cnt == 0) {
      return false;
    }
    cur = cur->c[x];
  }
  return cur->cnt > 0;
}

int count_occurences(Node* root, int val) {
  Node* cur = root;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    if (cur->c[x] == nullptr || cur->c[x]->cnt == 0) return 0;
    cur = cur->c[x];
  }
  return cur->cnt;
}

void erase(Node* root, int val) {
  if (!search(root, val)) return;
  Node* cur = root;
  for (int i = LOG_MAX; i >= 0; i--) {
    int x = (val >> i) & 1;
    cur = cur->c[x];
    cur->cnt--;
  }
}

void update(Node* root, int old_val, int new_val) {
  if (!search(root, old_val)) return;
  erase(root, old_val);
  insert(root, new_val);
}