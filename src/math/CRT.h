/*
   GENERALIZED CHINESE REMAINDER THEOREM (CRT)
   Idea:
   - Solves a system of linear congruences:
       x = a_1 (mod m_1)
       x = a_2 (mod m_2)
       ...
       x = a_n (mod m_n)
   - Unlike the standard CRT, this GENERALIZED version does NOT require 
     the moduli (m_i) to be pairwise coprime.
   - It works by iteratively merging two congruences into one.
   Math:
   - Merging x = a1 (mod m1) and x = a2 (mod m2):
     We need x = a1 + k1 * m1 = a2 + k2 * m2
     => k1 * m1 - k2 * m2 = a2 - a1
     This is a linear Diophantine equation. Let g = gcd(m1, m2).
     If (a2 - a1) is not divisible by g, there is NO SOLUTION.
     Otherwise, we solve for k1 and find the new merged congruence:
     x = new_a (mod lcm(m1, m2)).
   Complexity: O(N log(LCM))
   Note: Uses __int128_t to prevent overflow during intermediate multiplications.
*/
long long extgcd(long long a, long long b, long long &x, long long &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  long long x1, y1;
  long long d = extgcd(b, a % b, x1, y1);
  x = y1;
  y = x1 - y1 * (a / b);
  return d;
}

// Generalized CRT
// Inputs: vector of remainders `a`, vector of moduli `m`.
// Returns: A pair {ans_a, ans_m} such that x = ans_a (mod ans_m).
// If no solution exists, returns {-1, -1}.
pair<long long, long long> crt(const vector<long long>& a, const vector<long long>& m) {
  assert(a.size() == m.size());
  if (a.empty()) return {0, 1}; // Trivial case
  
  long long ans_a = a[0];
  long long ans_m = m[0];
  
  for (int i = 1; i < (int)a.size(); i++) {
    long long cur_a = a[i];
    long long cur_m = m[i];
    
    long long u, v;
    long long g = extgcd(ans_m, cur_m, u, v);
    
    // Check if a solution exists for this pair
    if ((cur_a - ans_a) % g != 0) {
      return {-1, -1}; // No solution
    }
    
    long long lcm = (ans_m / g) * cur_m;
    
    // Calculate k1 = (a2 - a1) / g * u (mod m2 / g)
    // Using __int128_t to avoid overflow when multiplying (cur_a - ans_a) / g and u
    long long diff = cur_a - ans_a;
    __int128_t k = (__int128_t)diff / g * u;
    k %= (cur_m / g);
    
    // Substitute k back to find the new remainder
    ans_a = ans_a + (long long)k * ans_m;
    
    // Normalize ans_a to be positive and within [0, lcm - 1]
    ans_a = (ans_a % lcm + lcm) % lcm;
    ans_m = lcm;
  }
  
  return {ans_a, ans_m};
}

/*
   USAGE EXAMPLE:
   
   signed main() {
     // System of equations:
     // x = 2 (mod 3)
     // x = 3 (mod 5)
     // x = 2 (mod 7)
     
     vector<long long> a = {2, 3, 2};
     vector<long long> m = {3, 5, 7};
     
     pair<long long, long long> res = crt(a, m);
     
     if (res.first == -1) {
       cout << "No solution exists.\n";
     } else {
       cout << "Smallest positive solution: x = " << res.first << "\n";
       cout << "General solution: x = " << res.first << " (mod " << res.second << ")\n";
     }
     
     // Output: 
     // Smallest positive solution: x = 23
     // General solution: x = 23 (mod 105)
     
     return 0;
   }
*/