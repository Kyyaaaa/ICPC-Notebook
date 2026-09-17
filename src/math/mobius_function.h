/*
   MOBIUS FUNCTION & INVERSION
   
   Definition of mu(n):
   - mu(n) = 1  if n = 1
   - mu(n) = 0  if n has a squared prime factor (e.g., 4, 9, 12, 18)
   - mu(n) = 1  if n is square-free with an EVEN number of prime factors
   - mu(n) = -1 if n is square-free with an ODD number of prime factors
   
   Important Properties for CP:
   1. Sum over divisors: The sum of mu(d) for all divisors d of n is 1 
      if n = 1, and 0 otherwise. (Sum_{d|n} mu(d) = [n == 1])
      -> This is the most used property to replace [gcd(i, j) == 1].
   2. Mobius Inversion Formula:
      If g(n) = Sum_{d|n} f(d), then f(n) = Sum_{d|n} mu(d) * g(n/d).
*/
// 1. Single Computation
// Computes mu(n) in O(sqrt(n)) time.
int get_mu(long long n) {
  if (n == 1) return 1;
  int prime_count = 0;
  
  for (long long i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      int count = 0;
      while (n % i == 0) {
        count++;
        n /= i;
      }
      if (count > 1) return 0; 
      prime_count++;
    }
  }
  if (n > 1) {
    prime_count++;
  }
  
  return (prime_count % 2 == 1) ? -1 : 1;
}

// 2. Linear Sieve Computation (Batch Computation)
// Computes mu(i) for all 1 <= i <= N in exactly O(N) time.
struct MobiusSieve {
  int n;
  vector<int> mu;
  vector<int> primes;
  vector<bool> is_prime;

  MobiusSieve(int _n) {
    n = _n;
    mu.assign(n + 1, 0);
    is_prime.assign(n + 1, true);
    
    is_prime[0] = is_prime[1] = false;
    mu[1] = 1;
    
    for (int i = 2; i <= n; i++) {
      if (is_prime[i]) {
        primes.push_back(i);
        mu[i] = -1; // Prime numbers have exactly 1 prime factor (ODD)
      }
      for (int p : primes) {
        if (i * p > n) break;
        is_prime[i * p] = false;
        
        if (i % p == 0) {
          // i * p is divisible by p^2
          mu[i * p] = 0;
          break;
        } else {
          // p is a new distinct prime factor
          mu[i * p] = -mu[i];
        }
      }
    }
  }
};

/*
   USAGE EXAMPLE:
   
   signed main() {
     // 1. Single query for a large number
     long long n = 30; // 30 = 2 * 3 * 5 (3 prime factors -> odd -> mu = -1)
     cout << "mu(" << n << ") = " << get_mu(n) << "\n"; // Output: -1
     
     long long m = 12; // 12 = 2^2 * 3 (has squared prime factor 2^2 -> mu = 0)
     cout << "mu(" << m << ") = " << get_mu(m) << "\n"; // Output: 0
     
     // 2. Precomputing mu for all numbers up to MAX_N in O(N)
     int MAX_N = 100000;
     MobiusSieve sieve(MAX_N);
     
     cout << "mu(10) = " << sieve.mu[10] << "\n"; // 10 = 2 * 5 -> Output: 1
     cout << "mu(97) = " << sieve.mu[97] << "\n"; // 97 is prime -> Output: -1
     
     return 0;
   }
*/