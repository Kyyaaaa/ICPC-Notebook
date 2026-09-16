/*
   SUFFIX ARRAY & LCP ARRAY (O(N log N) Construction)
   Idea:
   - Suffix Array (sa): Sorts all suffixes of a string lexicographically. 
     sa[i] stores the starting index of the i-th lexicographically 
     smallest suffix.
   - LCP Array (lcp): Stores the Longest Common Prefix between consecutive 
     suffixes in the sorted Suffix Array. lcp[i] = LCP(sa[i], sa[i-1]).
   Algorithm:
   - Appends a special character '$' (strictly smaller than all other characters) 
     to the end of the string.
   - Sorts substrings of length 2^k using Radix Sort in O(N).
   - Repeats log(N) times -> Total time O(N log N).
   - Computes LCP in O(N) using Kasai's Algorithm.
*/

struct SuffixArray {
  int n;
  string s;
  vector<int> sa;  // Suffix Array
  vector<int> c;   // Equivalence classes
  vector<int> lcp; // Longest Common Prefix array

  // Constructor automatically builds SA and LCP
  SuffixArray(string const& _s) : s(_s + "$") {
    n = s.length();
    sa.assign(n, 0);
    c.assign(n, 0);
    lcp.assign(n, 0);
    
    build_sa();
    build_lcp();
  }

  void build_sa() {
    const int alphabet = 256;
    vector<int> cnt(max(alphabet, n), 0);
    vector<int> p(n), pn(n), cn(n);
    
    // 1. Initial sort for length 1 (k = 0)
    for (int i = 0; i < n; i++) cnt[s[i]]++;
    for (int i = 1; i < alphabet; i++) cnt[i] += cnt[i - 1];
    for (int i = 0; i < n; i++) p[--cnt[s[i]]] = i;
    
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
      if (s[p[i]] != s[p[i - 1]]) classes++;
      c[p[i]] = classes - 1;
    }
    
    // 2. Transitions for length 2^h
    for (int h = 0; (1 << h) < n; h++) {
      // Shift by 2^h
      for (int i = 0; i < n; i++) {
        pn[i] = p[i] - (1 << h);
        if (pn[i] < 0) pn[i] += n;
      }
      
      // Radix Sort on the first half
      fill(cnt.begin(), cnt.begin() + classes, 0);
      for (int i = 0; i < n; i++) cnt[c[pn[i]]]++;
      for (int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
      for (int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
      
      // Update equivalence classes
      cn[p[0]] = 0;
      classes = 1;
      for (int i = 1; i < n; i++) {
        pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
        pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
        if (cur != prev) classes++;
        cn[p[i]] = classes - 1;
      }
      c.swap(cn);
    }
    sa = p;
  }

  // Kasai's algorithm to build LCP array in O(N)
  void build_lcp() {
    vector<int> rank(n, 0);
    for (int i = 0; i < n; i++) {
      rank[sa[i]] = i;
    }
    
    int k = 0;
    // Iterate through original string indices (excluding the appended '$')
    for (int i = 0; i < n - 1; i++) {
      int pi = rank[i];
      int j = sa[pi - 1]; // The suffix preceding the current one in SA
      
      // Expand the matching prefix
      while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
        k++;
      }
      
      lcp[pi] = k;
      if (k > 0) k--; // Move to the next suffix, LCP decreases by at most 1
    }
  }
};

/*
   USAGE EXAMPLES:
   
   signed main() {
     string str = "ababba";
     SuffixArray sa(str);
     
     // 1. Number of Distinct Substrings
     // Formula: Total substrings - sum of LCP array
     // Total substrings of length N is N * (N + 1) / 2
     long long N = str.length();
     long long distinct_substrings = N * (N + 1) / 2;
     for (int i = 1; i <= N; i++) {
       distinct_substrings -= sa.lcp[i];
     }
     cout << "Distinct substrings: " << distinct_substrings << "\n";
     
     // 2. Printing Suffix Array & LCP Array
     cout << "i\tSA[i]\tLCP[i]\tSuffix\n";
     // Note: sa.sa[0] is always the appended '$' (length 0 suffix)
     for (int i = 1; i <= N; i++) { 
       cout << i << "\t" 
            << sa.sa[i] << "\t" 
            << sa.lcp[i] << "\t" 
            << str.substr(sa.sa[i]) << "\n";
     }
     
     return 0;
   }
*/