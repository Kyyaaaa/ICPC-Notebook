/*
   EULER'S TOTIENT FUNCTION (PHI FUNCTION)
   Idea:
   - phi(n) counts the number of integers between 1 and n inclusive 
     that are coprime to n (i.e., gcd(x, n) == 1).
   Important Properties for CP:
   1. If p is prime: phi(p) = p - 1
   2. If p is prime and k >= 1: phi(p^k) = p^k - p^(k-1)
   3. Multiplicative: If gcd(a, b) == 1, then phi(a * b) = phi(a) * phi(b)
   4. Euler's Theorem: If gcd(a, m) == 1, then a^phi(m) = 1 (mod m)
   5. Sum of phi over divisors: The sum of phi(d) for all divisors d of n 
      is exactly equal to n. (Sum_{d|n} phi(d) = n)
*/
// 1. Single Computation
// Computes phi(n) in O(sqrt(n)) time.
// Uses the formula: phi(n) = n * (1 - 1/p1) * (1 - 1/p2) * ... 
// where p_i are the distinct prime factors of n.
long long get_phi(long long n) {
  long long result = n;
  for (long long i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      while (n % i == 0) {
        n /= i;
      }
      result -= result / i;
    }
  }
  if (n > 1) {
    result -= result / n;
  }
  return result;
}

// 2. Sieve Computation (Batch Computation)
// Computes phi(i) for all 1 <= i <= N in O(N log log N) time.
// Similar to the Sieve of Eratosthenes.
vector<int> phi_sieve(int n) {
  vector<int> phi(n + 1);
  
  for (int i = 0; i <= n; i++) {
    phi[i] = i;
  }
  
  for (int i = 2; i <= n; i++) {
    if (phi[i] == i) {
      for (int j = i; j <= n; j += i) {
        phi[j] -= phi[j] / i;
      }
    }
  }
  return phi;
}

/*
   USAGE EXAMPLE:
   signed main() {
     // 1. Single query for a large number
     long long n = 12;
     cout << "phi(" << n << ") = " << get_phi(n) << "\n"; 
     // Output: 4 (Coprimes are 1, 5, 7, 11)
     // 2. Precomputing phi for all numbers up to MAX_N
     int MAX_N = 100000;
     vector<int> phi = phi_sieve(MAX_N);
     cout << "phi(10) = " << phi[10] << "\n"; // Output: 4
     cout << "phi(97) = " << phi[97] << "\n"; // Output: 96 (Since 97 is prime)
     return 0;
   }
*/