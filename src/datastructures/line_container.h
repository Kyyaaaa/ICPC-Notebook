/*
   DYNAMIC CONVEX HULL TRICK (LINE CONTAINER 
   Idea:
   - Maintains the upper envelope of a set of lines y = k*x + m.
   - Allows dynamically adding lines and querying the MAXIMUM y value at 
     a given x in O(log N) time.
   - For MINIMUM queries: insert(-k, -m) and negate the query result: -query(x).
   How it works (KACTL approach):
   - Uses std::multiset to keep lines sorted by slope 'k'.
   - Maintains the intersection point 'p' with the next line.
   - Uses C++14 heterogeneous lookup (less<>) to binary search by 'x' value 
     using the 'p' attribute.
   Complexity:
   - Add line: Amortized O(log N)
   - Query: O(log N)
*/
const long long LINF = 2e18;
struct Line {
  mutable long long k, m, p;
  bool operator<(const Line& o) const { return k < o.k; }
  bool operator<(long long x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
  long long div(long long a, long long b) {
    return a / b - ((a ^ b) < 0 && a % b);
  }

  bool isect(iterator x, iterator y) {
    if (y == end()) { x->p = LINF; return false; }
    if (x->k == y->k) {
      x->p = x->m > y->m ? LINF : -LINF;
    } else {
      x->p = div(y->m - x->m, x->k - y->k);
    }
    return x->p >= y->p;
  }

  // Adds a line y = k*x + m to the envelope
  void add(long long k, long long m) {
    auto z = insert({k, m, 0}), y = z++, x = y;
    while (isect(y, z)) z = erase(z);
    if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
    while ((y = x) != begin() && (--x)->p >= y->p) {
      isect(x, erase(y));
    }
  }

  // Queries the maximum y value at a given x
  long long query(long long x) {
    assert(!empty());
    auto l = *lower_bound(x);
    return l.k * x + l.m;
  }
};

/*
   USAGE EXAMPLE:
   signed main() {
     LineContainer lc;
     // ---------------------------------------------------------
     // EXAMPLE 1: MAXIMUM QUERIES (Default behavior)
     // ---------------------------------------------------------
     // Add lines: y = 2x + 3, y = 1x + 10, y = 3x - 5
     lc.add(2, 3);
     lc.add(1, 10);
     lc.add(3, -5);
     
     cout << "Max at x=2: " << lc.query(2) << "\n"; // y = 1*(2) + 10 = 12
     cout << "Max at x=10: " << lc.query(10) << "\n"; // y = 3*(10) - 5 = 25
     
     // ---------------------------------------------------------
     // EXAMPLE 2: MINIMUM QUERIES (Trick)
     // ---------------------------------------------------------
     LineContainer min_lc;
     
     // We want to find MINIMUM of: y = 2x + 3, y = 1x + 10, y = 3x - 5
     // Trick: Insert (-k, -m)
     min_lc.add(-2, -3);
     min_lc.add(-1, -10);
     min_lc.add(-3, 5); // -(-5) = 5
     
     // Trick: Negate the query result
     cout << "Min at x=2: " << -min_lc.query(2) << "\n";  // y = 2*(2) + 3 = 7
     cout << "Min at x=10: " << -min_lc.query(10) << "\n"; // y = 1*(10) + 10 = 20
     
     return 0;
   }
*/