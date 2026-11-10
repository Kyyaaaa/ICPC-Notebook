/*
   INTERVAL CONTAINER (DISJOINT INTERVALS SET)
   Idea:
   - Maintains a set of disjoint intervals [L, R].
   - Automatically merges overlapping or adjacent intervals when adding.
   - Automatically splits intervals when removing a segment.
   - Dynamically maintains the total length/count of covered points.
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
    auto it = st.lower_bound({L - 1, -LINF});
    if (it != st.begin()) {
      auto prev = it; 
      --prev;
      if (prev->second >= L - 1) {
        it = prev;
      }
    }

    long long nl = L, nr = R;
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
    auto it = st.lower_bound({L, -LINF});
    if (it != st.begin()) {
      auto prev = it; 
      --prev;
      if (prev->second >= L) {
        it = prev;
      }
    }

    while (it != st.end() && it->first <= R) {
      long long il = it->first;
      long long ir = it->second;
      total_length -= (ir - il + 1);
      it = st.erase(it);

      if (il < L) {
        st.insert({il, L - 1});
        total_length += (L - 1 - il + 1);
      }
      
      if (ir > R) {
        st.insert({R + 1, ir});
        total_length += (ir - (R + 1) + 1);
      }
    }
  }

  // 3. Check if a specific point x is covered by any interval
  bool contains(long long x) {
    auto it = st.upper_bound({x, LINF});
    if (it == st.begin()) return false;
    --it;
    return it->second >= x;
  }
  
  // 4. Get total covered points length
  long long get_length() {
    return total_length;
  }
};