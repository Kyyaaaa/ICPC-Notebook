// Point coordinates use ll (long long).
// Dot, cross and len2 require their intermediate results to fit in ll.
struct Point {
  typedef ll T;
  T x, y;

  Point(T _x = 0, T _y = 0) : x(_x), y(_y) {}

  // Lexicographical order: x, then y
  bool operator<(Point p) const { return tie(x, y) < tie(p.x, p.y); }
  bool operator>(Point p) const { return tie(x, y) > tie(p.x, p.y); }
  bool operator==(Point p) const { return tie(x, y) == tie(p.x, p.y); }
  bool operator!=(Point p) const { return tie(x, y) != tie(p.x, p.y); }

  Point operator+(Point p) const { return Point(x + p.x, y + p.y); }
  Point operator-(Point p) const { return Point(x - p.x, y - p.y); }

  T operator*(Point p) const { return x * p.x + y * p.y; }  // Dot product
  T operator^(Point p) const { return x * p.y - y * p.x; }  // Cross product

  Point operator*(T d) const { return Point(x * d, y * d); }
  Point operator/(T d) const { return Point(x / d, y / d); }  // Integer division

  T len2() const { return x * x + y * y; }
  double len() const { return hypot((double)x, (double)y); }

  Point perp() const { return Point(-y, x); }  // 90 degrees CCW

  friend ostream& operator<<(ostream &os, const Point &p) {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};

ll sgn(ll x) {
  return (x > 0) - (x < 0);
}

// > 0: CCW; < 0: CW; = 0: collinear
ll ccw(const Point &p0, const Point &p1, const Point &p2) {
  return (p1 - p0) ^ (p2 - p0);
}
