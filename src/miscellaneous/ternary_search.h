/*
   Idea:
   - Finds the minimum or maximum of a Unimodal function (a function that 
     strictly increases then strictly decreases, or vice-versa).
   Two variants:
   1. Floating-point: Iterate a fixed number of times (e.g., 200) instead 
      of testing (R - L > eps). 200 iterations shrink the search space by 
      (2/3)^200, which is enough for any floating-point precision limit.
   2. Integer: Search until the range [L, R] is small (e.g., length <= 3), 
      then use a simple linear scan. This avoids infinite loops caused by 
      integer division rounding.
*/
// The unimodal function we want to optimize.
// Example: f(x) = (x - 5)^2 + 10 (Minimum is at x = 5)
double f_double(double x) {
  return (x - 5.0) * (x - 5.0) + 10.0;
}

long long f_int(long long x) {
  return (x - 5) * (x - 5) + 10;
}

struct TernarySearch {
  // 1. Ternary Search on Real Numbers (Floating-point)
  // Returns the x-coordinate of the MINIMUM.
  // To find MAXIMUM: change `if (f_double(m1) < f_double(m2))` to `>`
  double solve_double(double L, double R) {
    // 200 iterations are ALWAYS enough to achieve maximum double precision
    for (int i = 0; i < 200; i++) {
      double m1 = L + (R - L) / 3.0;
      double m2 = R - (R - L) / 3.0;
      
      if (f_double(m1) < f_double(m2)) {
        R = m2; // The minimum is closer to m1, discard [m2, R]
      } else {
        L = m1; // The minimum is closer to m2, discard [L, m1]
      }
    }
    return L; // L and R are practically identical now
  }

  // 2. Ternary Search on Integers
  // Returns the x-coordinate of the MINIMUM.
  // To find MAXIMUM: change `<` to `>` in BOTH the ternary condition 
  // and the final linear scan.
  long long solve_int(long long L, long long R) {
    // Stop when the range is small enough (3 points or fewer)
    while (R - L > 2) {
      long long m1 = L + (R - L) / 3;
      long long m2 = R - (R - L) / 3;
      
      if (f_int(m1) < f_int(m2)) {
        R = m2;
      } else {
        L = m1;
      }
    }
    
    // Linear scan for the remaining elements to avoid integer rounding bugs
    long long best_x = L;
    long long best_val = f_int(L);
    for (long long i = L + 1; i <= R; i++) {
      long long current_val = f_int(i);
      if (current_val < best_val) {
        best_val = current_val;
        best_x = i;
      }
    }
    
    return best_x;
  }
};

/*
   USAGE EXAMPLE:
   signed main() {
     TernarySearch ts;
     
     // Floating point search in range [-1000.0, 1000.0]
     double ans_d = ts.solve_double(-1000.0, 1000.0);
     cout << fixed << setprecision(6);
     cout << "Min (Double) at x = " << ans_d << ", f(x) = " << f_double(ans_d) << "\n";
     
     // Integer search in range [-1000, 1000]
     long long ans_i = ts.solve_int(-1000, 1000);
     cout << "Min (Int) at x = " << ans_i << ", f(x) = " << f_int(ans_i) << "\n";
     
     // Expected output for both: 
     // x = 5, f(x) = 10
     
     return 0;
   }
*/