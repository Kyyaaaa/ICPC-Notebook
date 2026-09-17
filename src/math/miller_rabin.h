/*
   MILLER-RABIN PRIMALITY TEST
   
   Idea:
   - A fast primality test. While originally probabilistic, it can be made 
     DETERMINISTIC for all 64-bit integers by testing a specific, small set 
     of prime bases.
   - To test if n is prime, we write n - 1 = d * 2^s, where d is odd.
   - For a base 'a', n is composite if a^d != 1 (mod n) AND a^(d * 2^r) != n - 1 (mod n) 
     for all r in [0, s - 1].
     
   Bases for Deterministic Test:
   - For n < 2^64, testing the first 12 primes is strictly sufficient:
     {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}
     
   Complexity:
   - Time: O(K log n) where K is the number of bases (K <= 12).
   - Space: O(1)
   - Uses __uint128_t to cleanly prevent overflow during 64-bit multiplications.
*/
using u64 = uint64_t;
using u128 = __uint128_t;

// Modular multiplication using 128-bit integer to avoid overflow
u64 mod_mul(u64 a, u64 b, u64 m) {
  return (u64)((u128)a * b % m);
}

// Modular exponentiation: (base^exp) % mod
u64 mod_pow(u64 base, u64 exp, u64 mod) {
  u64 res = 1;
  base %= mod;
  while (exp > 0) {
    if (exp % 2 == 1) res = mod_mul(res, base, mod);
    base = mod_mul(base, base, mod);
    exp /= 2;
  }
  return res;
}

// Miller-Rabin deterministic primality test for 64-bit integers
bool is_prime(u64 n) {
  if (n < 2) return false;
  if (n == 2 || n == 3) return true;
  if (n % 2 == 0) return false;

  // Write n - 1 as d * 2^s
  u64 d = n - 1;
  int s = 0;
  while (d % 2 == 0) {
    d /= 2;
    s++;
  }

  // Deterministic bases for values up to 2^64
  static const u64 bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

  for (u64 a : bases) {
    // If n is smaller than or equal to the base, we are done
    // (since n is prime and has already passed the n % 2 == 0 check, 
    // it would just be one of the prime bases).
    if (n <= a) break; 
    
    u64 x = mod_pow(a, d, n);
    if (x == 1 || x == n - 1) continue;

    bool composite = true;
    for (int r = 1; r < s; r++) {
      x = mod_mul(x, x, n);
      if (x == n - 1) {
        composite = false;
        break;
      }
    }
    
    if (composite) return false;
  }
  
  return true;
}

/*
   USAGE EXAMPLE:
   
   signed main() {
     long long n = 100000000000000003LL; // A large 64-bit number
     
     if (is_prime(n)) {
       cout << n << " is PRIME.\n";
     } else {
       cout << n << " is COMPOSITE.\n";
     }
     
     // Expected Output: 100000000000000003 is PRIME.
     
     return 0;
   }
*/