/*
   XOR BASIS (Linear Basis over GF(2))
   
   Idea:
   - Represents a set of numbers using a minimal set of linearly independent 
     numbers (basis).
   - Any XOR sum of a subset in the original set can be formed by a XOR sum 
     of a subset of the basis.
   - The size of the basis never exceeds the number of bits (LOG_D).
   
   Complexity:
   - Insert / Check / Get Max: O(LOG_D)
   - Merge two bases: O(LOG_D^2)
*/
const int LOG_D = 60; // 60 for values up to 10^18, 30 for values up to 10^9

struct XorBasis {
  long long basis[LOG_D];
  int sz; // Number of elements currently in the basis

  XorBasis() {
    memset(basis, 0, sizeof(basis));
    sz = 0;
  }

  // 1. Insert a value. Returns true if successfully inserted 
  // (i.e., the value is linearly independent from the current basis).
  bool insert(long long x) {
    for (int i = LOG_D - 1; i >= 0; i--) {
      if ((x >> i) & 1) {
        if (!basis[i]) {
          basis[i] = x;
          sz++;
          return true;
        }
        x ^= basis[i];
      }
    }
    return false; // x became 0, meaning it was already span by the basis
  }

  // 2. Check if a value can be formed by XORing a subset of the basis
  bool check(long long x) {
    for (int i = LOG_D - 1; i >= 0; i--) {
      if ((x >> i) & 1) {
        if (!basis[i]) return false;
        x ^= basis[i];
      }
    }
    return true; // x became 0, so it can be formed
  }

  // 3. Get the maximum possible XOR sum using the basis.
  // Optionally, you can pass an initial value (start_val).
  long long get_max(long long start_val = 0) {
    long long res = start_val;
    for (int i = LOG_D - 1; i >= 0; i--) {
      // If XORing with basis[i] makes the result larger, do it
      if ((res ^ basis[i]) > res) {
        res ^= basis[i];
      }
    }
    return res;
  }

  // 4. Merge this basis with another basis (Very useful for Segment Trees)
  void merge(const XorBasis& other) {
    for (int i = 0; i < LOG_D; i++) {
      if (other.basis[i]) {
        insert(other.basis[i]);
      }
    }
  }
};

/*
   USAGE EXAMPLE:
   
   signed main() {
     int n = 5;
     long long a[] = {10, 15, 20, 25, 30};
     
     XorBasis xb;
     
     // Build basis
     for (int i = 0; i < n; i++) {
       xb.insert(a[i]);
     }
     
     // 1. Find the maximum XOR sum of any subset
     cout << "Max XOR sum: " << xb.get_max() << "\n";
     
     // 2. Find the maximum XOR sum if we initially have value 100
     cout << "Max XOR with 100: " << xb.get_max(100) << "\n";
     
     // 3. Check if 5 can be formed by a subset XOR
     if (xb.check(5)) {
       cout << "5 can be formed\n";
     } else {
       cout << "5 cannot be formed\n";
     }
     
     return 0;
   }
*/