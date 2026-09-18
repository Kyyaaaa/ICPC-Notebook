/*
   INTERVAL CONTAINER (DISJOINT INTERVALS SET)
   Idea:
   - Maintains a set of disjoint intervals [L, R].
   - Automatically merges overlapping or adjacent intervals when adding.
   - Automatically splits intervals when removing a segment.
   - Dynamically maintains the total length/count of covered points.
   Complexity:
   - add_interval: Amortized O(log N)
   - remove_interval: Amortized O(log N)
   - contains: O(log N)
*/
const long long LINF = 2e18;

struct IntervalContainer {
  // Stores intervals as {L, R}. 
  // R is inclusive. For example, [1, 3] covers points 1, 2, and 3.
  set<pair<long long, long long>> st;
  long long total_length = 0;

  // 1. Add an interval [L, R] and merge with overlapping/adjacent ones
  void add_interval(long long L, long long R) {
    if (L > R) return;
    
    // Find the first interval that ends at or after L - 1
    // (We use L - 1 because we want to merge adjacent intervals like [1, 2] and [3, 4])
    auto it = st.lower_bound({L - 1, -LINF});
    if (it != st.begin()) {
      auto prev = it; 
      --prev;
      if (prev->second >= L - 1) {
        it = prev;
      }
    }

    long long nl = L, nr = R;
    // Iterate and erase all intervals that overlap with [L, R]
    // (We use R + 1 to merge adjacent intervals)
    while (it != st.end() && it->first <= R + 1) {
      nl = min(nl, it->first);
      nr = max(nr, it->second);
      total_length -= (it->second - it->first + 1);
      it = st.erase(it); // C++11: erase returns the iterator to the next element
    }
    
    st.insert({nl, nr});
    total_length += (nr - nl + 1);
  }

  // 2. Remove an interval [L, R] (punch a hole)
  void remove_interval(long long L, long long R) {
    if (L > R) return;
    
    // Find the first interval that ends at or after L
    auto it = st.lower_bound({L, -LINF});
    if (it != st.begin()) {
      auto prev = it; 
      --prev;
      if (prev->second >= L) {
        it = prev;
      }
    }

    // Process all intervals that overlap with [L, R]
    while (it != st.end() && it->first <= R) {
      long long il = it->first;
      long long ir = it->second;
      total_length -= (ir - il + 1);
      it = st.erase(it);

      // If the original interval extends to the left of the removed segment, keep that part
      if (il < L) {
        st.insert({il, L - 1});
        total_length += (L - 1 - il + 1);
      }
      
      // If the original interval extends to the right of the removed segment, keep that part
      // Note: The new right part has a left bound > R, so it won't be processed again by the while loop.
      if (ir > R) {
        st.insert({R + 1, ir});
        total_length += (ir - (R + 1) + 1);
      }
    }
  }

  // 3. Check if a specific point x is covered by any interval
  bool contains(long long x) {
    // upper_bound finds the first interval whose L is strictly greater than x
    auto it = st.upper_bound({x, LINF});
    if (it == st.begin()) return false;
    --it; // The interval before it has L <= x
    return it->second >= x;
  }
  
  // 4. Get total covered points length
  long long get_length() {
    return total_length;
  }
};

/*
   USAGE EXAMPLE:
   signed main() {
     IntervalContainer ic;
     
     ic.add_interval(1, 5);
     ic.add_interval(7, 10);
     cout << "Length: " << ic.get_length() << "\n"; // (5-1+1) + (10-7+1) = 5 + 4 = 9
     
     // Merge adjacent and overlapping segments
     // Adding [4, 8] will merge [1, 5] and [7, 10] into [1, 10]
     ic.add_interval(4, 8);
     cout << "Length: " << ic.get_length() << "\n"; // Output: 10
     
     // Punch a hole in the middle
     ic.remove_interval(3, 6);
     cout << "Length: " << ic.get_length() << "\n"; // Remainder: [1, 2] and [7, 10] -> Length: 6
     
     // Check points
     cout << "Contains 2? " << ic.contains(2) << "\n"; // Output: 1 (true)
     cout << "Contains 5? " << ic.contains(5) << "\n"; // Output: 0 (false)
     
     return 0;
   }
*/