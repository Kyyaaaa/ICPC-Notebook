// Requires Point, ccw and sgn from geometry_point.h.
// Integer dot/cross computations must fit in ll.
const ll LINF = 2e18;

// Inclusive: endpoints count. Also handles a degenerate segment p0 == p1.
bool on_segment(const Point &p, const Point &p0, const Point &p1) {
  return ccw(p0, p1, p) == 0
    && min(p0.x, p1.x) <= p.x && p.x <= max(p0.x, p1.x)
    && min(p0.y, p1.y) <= p.y && p.y <= max(p0.y, p1.y);
}

// Shortest distance from p to the closed segment [p0, p1].
double dist_segment(const Point &p, const Point &p0, const Point &p1) {
  if ((p1 - p0) * (p - p1) >= 0) return (p - p1).len();
  if ((p0 - p1) * (p - p0) >= 0) return (p - p0).len();
  return abs((double)((p1 - p0) ^ (p - p0))) / (p1 - p0).len();
}

// Strict point-in-convex-polygon test in O(log N).
// Preconditions: strictly convex polygon, >= 3 vertices, CLOCKWISE order,
// no repeated first vertex at the end. Boundary is outside.
// convex_hull() returns CCW, so reverse its output before calling this.
bool inside_convex(const Point &p, const vector<Point> &poly) {
  int n = (int)poly.size();
  if (n < 3) return false;
  if (ccw(poly[0], poly[1], p) >= 0) return false;
  if (ccw(poly[n - 1], poly[0], p) >= 0) return false;

  int l = 1, r = n - 1;
  while (l < r) {
    int mid = (l + r + 1) / 2;
    if (ccw(poly[0], p, poly[mid]) >= 0) {
      l = mid;
    } else {
      r = mid - 1;
    }
  }
  return ccw(poly[l], p, poly[l + 1]) > 0;
}

// Winding-number test for a simple polygon, CW or CCW, in O(N).
// Return: 0 = outside, LINF = on boundary, otherwise strictly inside.
// For a CCW simple polygon the interior winding number is positive;
// for CW it is negative.
ll wn_poly(const Point &p, const vector<Point> &poly) {
  ll wn = 0;
  int n = (int)poly.size();

  for (int i = 0; i < n; i++) {
    const Point &a = poly[i], &b = poly[(i + 1) % n];
    if (on_segment(p, a, b)) return LINF;

    if (a.y <= p.y) {
      if (b.y > p.y && ccw(a, b, p) > 0) ++wn;
    } else {
      if (b.y <= p.y && ccw(a, b, p) < 0) --wn;
    }
  }
  return wn;
}

// Inclusive segment intersection: touching endpoints/overlapping count.
// Handles degenerate segments (a == b or c == d).
bool segment_intersect(const Point &a, const Point &b, const Point &c, const Point &d) {
  ll o1 = ccw(a, b, c), o2 = ccw(a, b, d);
  ll o3 = ccw(c, d, a), o4 = ccw(c, d, b);

  if (o1 == 0 && on_segment(c, a, b)) return true;
  if (o2 == 0 && on_segment(d, a, b)) return true;
  if (o3 == 0 && on_segment(a, c, d)) return true;
  if (o4 == 0 && on_segment(b, c, d)) return true;

  return sgn(o1) * sgn(o2) < 0 && sgn(o3) * sgn(o4) < 0;
}