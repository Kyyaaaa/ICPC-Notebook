/*
  - For a boolean variable x, we create two nodes: x (1..n) and NOT(x) (n+1..2n).
  - A clause (x OR y) is equivalent to: (!x => y) and (!y => x).
  - If x and NOT(x) are in the same Strongly Connected Component (SCC), 
    it's a contradiction -> NO solution.
  - We assign TRUE to x if its SCC is topologically sorted AFTER NOT(x)'s SCC.
    (Tarjan assigns SCC IDs in reverse topological order, so col[x] < col[NOT(x)])
*/
/*
2-SAT Implication Graph (x, y are literals):

x OR y        : NOT(x) -> y,      NOT(y) -> x
x AND y       : NOT(x) -> x,      NOT(y) -> y
x => y        : x -> y,           NOT(y) -> NOT(x)
x == y        : x -> y,           y -> x,
                NOT(x) -> NOT(y), NOT(y) -> NOT(x)
x != y (XOR)  : x -> NOT(y),      y -> NOT(x),
                NOT(x) -> y,      NOT(y) -> x
NOT(x AND y)  : x -> NOT(y),      y -> NOT(x)

Force x = 1   : NOT(x) -> x
Force x = 0   : x -> NOT(x)

IMPORTANT:
- Each implication a -> b must also have NOT(b) -> NOT(a).
- SAT iff col[x] != col[NOT(x)] for every variable x.
*/

const int N = 2e4 + 5;

int n;
vector<int> G[2 * N];
int num[2 * N], low[2 * N], cnt;
int col[2 * N], ct;
vector<int> stk;
int ans[N];

int NOT(int x) {
  return (x <= n ? x + n : x - n);
}

void addedge(int x, int y) {
  G[NOT(x)].push_back(y);
  G[NOT(y)].push_back(x);
}

void tarjan(int u) {
  num[u] = low[u] = ++cnt;
  stk.push_back(u);
  for (int v : G[u]) {
    if (!col[v]) {
      if (num[v]) {
        low[u] = min(low[u], num[v]);
      } else {
        tarjan(v);
        low[u] = min(low[u], low[v]);
      }
    }
  }
  if (num[u] == low[u]) {
    ct++;
    while (true) {
      int v = stk.back();
      stk.pop_back();
      col[v] = ct;
      if (v == u) break;
    }
  }
}

bool solve() {
  rep(i, 1, 2 * n) {
    if (!num[i]) tarjan(i);
  }
  rep(i, 1, n) {
    if (col[i] == col[NOT(i)]) return false;
    ans[i] = (col[i] < col[NOT(i)]);
  }
  return true;
}