// Andrew's monotone chain: O(N log N).
// Returns distinct extreme vertices in CCW order, without a repeated first vertex.
// Collinear points on hull edges are removed; all-collinear input -> 2 endpoints.
// For a hull containing EVERY boundary point, a different collinear policy is needed.
vector<Point> convex_hull(vector<Point> dots) {
  sort(dots.begin(), dots.end());
  dots.erase(unique(dots.begin(), dots.end()), dots.end());
  if (dots.size() <= 1) return dots;

  vector<Point> A(1, dots[0]);
  const int n = (int)dots.size();
  for (int c = 0; c < 2; reverse(dots.begin(), dots.end()), c++) {
    for (int i = 1, t = (int)A.size(); i < n; A.emplace_back(dots[i++])) {
      while ((int)A.size() > t && ccw(A[A.size() - 2], A.back(), dots[i]) <= 0) {
        A.pop_back();
      }
    }
  }
  A.pop_back();
  return A;
}
