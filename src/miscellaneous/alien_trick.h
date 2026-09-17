/*
   ALIEN'S TRICK (WQS BINARY SEARCH / LAGRANGE RELAXATION)
   
   Idea:
   - Solves DP problems with an "exactly K items/partitions" constraint.
   - We drop the K constraint and add a penalty `lambda` for each partition.
   - We binary search on `lambda`. If we use > K partitions, we increase 
     the penalty. If we use < K partitions, we decrease it.
   - To handle collinear points on the convex hull, we always prefer the 
     MAXIMUM (or MINIMUM) number of partitions for the same cost, and use 
     `cnt >= K` in the binary search.
     
   Condition:
   - Let f(x) be the optimal cost using exactly x partitions.
   - f(x) MUST be convex or concave.
   
   Complexity: O(N * log(MAX_PENALTY)) instead of O(N * K).
*/
const long long LINF = 1e18;

int n;
vector<long long> a; // Input array

// 1. The 1D DP function without the 'K' constraint
// Returns a pair: {optimal_cost_with_penalty, number_of_partitions_used}
pair<long long, int> solve_dp(long long lambda) {
  // dp[i] stores {min_cost, max_partitions_used} up to index i
  vector<pair<long long, int>> dp(n + 1, {LINF, 0});
  dp[0] = {0, 0};
  
  for (int i = 1; i <= n; i++) {
    // Iterate over all possible previous split points
    // Note: In real problems, this inner loop is often optimized to O(1) 
    // using Convex Hull Trick, D&C, or Monotone Queue.
    for (int j = 0; j < i; j++) {
      // cost(j+1, i) is the cost of the segment from j+1 to i
      // We ADD `lambda` as a penalty for creating this new segment
      long long segment_cost = 0; /* TODO: Calculate real cost */
      long long new_cost = dp[j].first + segment_cost + lambda;
      int new_cnt = dp[j].second + 1;
      
      // We want to MINIMIZE cost. 
      // TIE-BREAKER: If costs are equal, MAXIMIZE the number of partitions.
      // This tie-breaker is CRUCIAL to handle collinear points on the convex hull!
      if (new_cost < dp[i].first || (new_cost == dp[i].first && new_cnt > dp[i].second)) {
        dp[i] = {new_cost, new_cnt};
      }
    }
  }
  return dp[n];
}

/*
   USAGE EXAMPLE:
   
   signed main() {
     n = 1000;
     int K = 50; // We need exactly K partitions
     
     // Binary search for the optimal penalty `lambda`
     // The range depends on the maximum possible cost difference
     long long l = -1e12, r = 1e12; 
     long long ans = -1;
     
     while (l <= r) {
       long long mid = l + (r - l) / 2;
       
       pair<long long, int> res = solve_dp(mid);
       long long cost = res.first;
       int cnt = res.second;
       
       // If we used >= K partitions, it means the penalty is small enough 
       // to allow at least K partitions. We record the answer and try to 
       // INCREASE the penalty to force fewer partitions.
       if (cnt >= K) {
         // The real cost is the DP cost MINUS the penalty we added for K partitions
         ans = cost - K * mid; 
         l = mid + 1; // Increase penalty
       } else {
         r = mid - 1; // Decrease penalty
       }
     }
     
     cout << "Minimum cost using exactly " << K << " partitions: " << ans << "\n";
     
     return 0;
   }
*/