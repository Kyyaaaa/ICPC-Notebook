/*
   LUCAS THEOREM
   Idea:
   - Computes nCr(n, k) % p where n and k can be very large (up to 10^18), 
     but p is a relatively small PRIME number (e.g., p <= 10^6).
   - Theorem: nCr(n, k) = Product of nCr(n_i, k_i) (mod p), where n_i and 
     k_i are the digits of n and k in base p.
   - If at any point k_i > n_i, then nCr(n_i, k_i) = 0, meaning the entire 
     result becomes 0.
   Complexity:
   - Precomputation: O(p) to compute factorials up to p - 1.
   - Query: O(log_p(n)) per nCr query.
   - Space: O(p)
   Requirement:
   - p MUST be a prime number.
*/
struct LucasTheorem {
  long long p;
  vector<long long> fact, invFact;

  long long power(long long base, long long exp) {
    long long res = 1;
    base %= p;
    while (exp > 0) {
      if (exp % 2 == 1) res = (res * base) % p;
      base = (base * base) % p;
      exp /= 2;
    }
    return res;
  }

  long long modInverse(long long n) {
    return power(n, p - 2);
  }

  // Constructor: Precomputes factorials and inverse factorials up to p - 1
  LucasTheorem(long long prime_mod) {
    p = prime_mod;
    fact.assign(p, 1);
    invFact.assign(p, 1);
    
    for (int i = 1; i < p; i++) {
      fact[i] = (fact[i - 1] * i) % p;
    }
    
    invFact[p - 1] = modInverse(fact[p - 1]);
    for (int i = p - 2; i >= 0; i--) {
      invFact[i] = (invFact[i + 1] * (i + 1)) % p;
    }
  }

  // Computes nCr(n, k) % p for small values (n, k < p)
  long long nCr_small(long long n, long long k) {
    if (k < 0 || k > n) return 0;
    long long num = fact[n];
    long long den = (invFact[k] * invFact[n - k]) % p;
    return (num * den) % p;
  }

  // Computes nCr(n, k) % p for large values using Lucas Theorem
  long long lucas(long long n, long long k) {
    if (k < 0 || k > n) return 0;
    
    long long res = 1;
    while (n > 0 || k > 0) {
      long long ni = n % p;
      long long ki = k % p;
      
      if (ki > ni) return 0; 
      
      res = (res * nCr_small(ni, ki)) % p;
      
      n /= p;
      k /= p;
    }
    return res;
  }
};