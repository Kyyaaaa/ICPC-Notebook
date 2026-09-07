// 1. Core Extended GCD
// Returns gcd(a, b) and updates x, y via reference.
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

// 2. Solve Linear Diophantine Equation: ax + by = c
// Returns true if a solution exists, false otherwise.
// If true, (x0, y0) stores one valid base solution, and g stores gcd(a, b).
//
// GENERAL SOLUTION GENERATOR:
// All solutions can be generated using an integer k:
// x = x0 + k * (b / g)
// y = y0 - k * (a / g)
bool solve_diophantine(long long a, long long b, long long c, long long &x0, long long &y0, long long &g) {
  // Solve for abs(a) and abs(b) first to maintain correct signs
  g = extgcd(abs(a), abs(b), x0, y0);
  
  if (c % g != 0) {
    return false; // No solution if c is not divisible by gcd(a, b)
  }
  
  x0 *= c / g;
  y0 *= c / g;
  
  // Adjust signs back
  if (a < 0) x0 = -x0;
  if (b < 0) y0 = -y0;
  
  return true;
}

// 3. Modular Multiplicative Inverse
// Solves: ax = 1 (mod m). 
// Valid ONLY IF a and m are coprime (gcd(a, m) == 1).
// Returns -1 if the inverse does not exist.
long long mod_inverse(long long a, long long m) {
  long long x, y;
  long long g = extgcd(a, m, x, y);
  
  if (g != 1) {
    return -1; // Inverse doesn't exist
  }
  
  // x can be negative, so we adjust it to be in the range [0, m - 1]
  return (x % m + m) % m;
}