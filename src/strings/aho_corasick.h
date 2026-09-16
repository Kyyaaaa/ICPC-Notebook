/*
   AHO-CORASICK AUTOMATON
   Idea:
   - Multi-pattern string search algorithm. 
   - Constructs a Trie from a set of patterns and adds:
     1. Failure Links (`fail`): Where to go when a character mismatch occurs.
     2. Output Links (`output`): Direct link to the nearest valid pattern 
        ending in the failure chain (handles overlapping/substring patterns).
   - OPTIMIZATION: We convert the Trie into a Deterministic Finite Automaton 
     (DFA). Transition `next[c]` either points to the actual child OR the 
     fallback state in O(1).
     
   Complexity:
   - Build Time: O(Sum of pattern lengths * Alphabet Size)
   - Search Time: O(Length of Text + Number of Matches)
*/

const int ALPHABET = 26;
const char BASE_CHAR = 'a'; // Change to 'A' or '0' depending on the problem

struct ACNode {
  int next[ALPHABET];
  int fail;
  int output; // Also known as dict_link (points to the nearest terminal node)
  vector<int> pattern_ids; // IDs of patterns that end exactly at this node

  ACNode() {
    fill(begin(next), end(next), 0);
    fail = 0;
    output = 0;
  }
};

struct AhoCorasick {
  vector<ACNode> t;

  AhoCorasick() {
    t.emplace_back(); // Root node at index 0
  }

  // 1. Insert a pattern into the Trie
  void insert(const string& s, int id) {
    int cur = 0;
    for (char c : s) {
      int v = c - BASE_CHAR;
      if (!t[cur].next[v]) {
        t[cur].next[v] = t.size();
        t.emplace_back();
      }
      cur = t[cur].next[v];
    }
    t[cur].pattern_ids.push_back(id);
  }

  // 2. Build Failure and Output links via BFS
  void build() {
    queue<int> q;
    
    // Initialize root's children
    for (int i = 0; i < ALPHABET; i++) {
      if (t[0].next[i]) {
        t[t[0].next[i]].fail = 0;
        q.push(t[0].next[i]);
      }
    }

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int i = 0; i < ALPHABET; i++) {
        int v = t[u].next[i];
        if (v) {
          // If child exists, set its failure link to the child of u's fail node
          t[v].fail = t[t[u].fail].next[i];
          
          // Optimization: Output link points to the nearest pattern end
          if (!t[t[v].fail].pattern_ids.empty()) {
            t[v].output = t[v].fail;
          } else {
            t[v].output = t[t[v].fail].output;
          }
          
          q.push(v);
        } else {
          // Trie Automaton Optimization: If child doesn't exist, 
          // point directly to the fallback state. This makes text traversal O(1).
          t[u].next[i] = t[t[u].fail].next[i];
        }
      }
    }
  }

  // 3. Search for patterns in the given text
  void search(const string& text) {
    int cur = 0;
    for (int i = 0; i < text.length(); i++) {
      int c = text[i] - BASE_CHAR;
      cur = t[cur].next[c]; // O(1) transition due to Automaton optimization

      // Traverse output links to find all patterns ending at current position i
      int temp = cur;
      while (temp != 0) {
        for (int id : t[temp].pattern_ids) {
          // You can store or process the match here
          cout << "Pattern ID " << id << " ends at index " << i << "\n";
        }
        // Jump to the next valid pattern in the failure chain
        // If the current node doesn't have an end word but has an output link,
        // this safely jumps to it.
        if (t[temp].pattern_ids.empty()) {
          temp = t[temp].output;
        } else {
          temp = t[temp].output;
        }
      }
    }
  }
};

/*
   USAGE EXAMPLE:
   
   signed main() {
     AhoCorasick ac;
     
     vector<string> patterns = {"he", "she", "his", "hers"};
     for (int i = 0; i < patterns.size(); i++) {
       ac.insert(patterns[i], i);
     }
     
     // MUST call build() after inserting all patterns and before searching
     ac.build();
     
     string text = "ushers";
     cout << "Searching in text: " << text << "\n";
     ac.search(text);
     
     // Output expected:
     // Pattern ID 1 ("she") ends at index 3
     // Pattern ID 0 ("he") ends at index 3
     // Pattern ID 3 ("hers") ends at index 5
     
     return 0;
   }
*/