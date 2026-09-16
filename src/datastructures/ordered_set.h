/*
   GNU PBDS ORDERED SET (Policy-Based Data Structure)
   
   Idea:
   - A balanced binary search tree (Red-Black Tree) with order statistics.
   - Works exactly like std::set but with two additional O(log N) functions:
     1. find_by_order(k): returns an iterator to the k-th smallest element (0-indexed).
     2. order_of_key(x): returns the number of elements strictly smaller than x.
     
   IMPORTANT FOR MULTISET:
   - Do NOT use `less_equal<T>` for multiset if you need to use `.erase()`. 
     It will delete ALL occurrences of the value, or break the tree structure.
   - The best and safest way to implement an ordered_multiset is to use 
     `pair<T, int>` where the second element is a unique counter/ID.
*/
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// 1. Ordered Set (Distinct elements only)
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// 2. Ordered Multiset (Allows duplicates safely by using pair<T, int>)
template <typename T>
using ordered_multiset = tree<pair<T, int>, null_type, less<pair<T, int>>, rb_tree_tag, tree_order_statistics_node_update>;

/*
   USAGE EXAMPLE:
   
   signed main() {
     // ---------------------------------------------------------
     // EXAMPLE 1: ORDERED SET (Distinct Elements)
     // ---------------------------------------------------------
     ordered_set<int> os;
     os.insert(10);
     os.insert(30);
     os.insert(20);
     os.insert(30); // Ignored, as 30 is already present
     
     // find_by_order(k): Returns iterator to the k-th smallest element (0-indexed)
     cout << *os.find_by_order(0) << "\n"; // Output: 10
     cout << *os.find_by_order(2) << "\n"; // Output: 30
     
     // order_of_key(x): Returns the number of elements STRICTLY smaller than x
     cout << os.order_of_key(25) << "\n";  // Output: 2 (10 and 20)
     cout << os.order_of_key(20) << "\n";  // Output: 1 (only 10)
     
     os.erase(20); // Erases 20 normally
     
     
     // ---------------------------------------------------------
     // EXAMPLE 2: ORDERED MULTISET (With Duplicates)
     // ---------------------------------------------------------
     ordered_multiset<int> oms;
     int timer = 0; // Unique ID counter to distinguish duplicates
     
     // Insert: use pair {value, ++timer}
     oms.insert({10, ++timer});
     oms.insert({20, ++timer});
     oms.insert({20, ++timer});
     oms.insert({30, ++timer});
     
     // order_of_key: just pass {value, -1} to get count of elements < value
     cout << oms.order_of_key({20, -1}) << "\n"; // Output: 1 (only {10})
     cout << oms.order_of_key({30, -1}) << "\n"; // Output: 3 ({10}, {20}, {20})
     
     // find_by_order works exactly the same, but returns a pair
     cout << oms.find_by_order(1)->first << "\n"; // Output: 20
     cout << oms.find_by_order(2)->first << "\n"; // Output: 20
     
     // Erase one occurrence of 20: 
     // Find its iterator using lower_bound with -1, then erase the iterator
     auto it = oms.lower_bound({20, -1});
     if (it != oms.end() && it->first == 20) {
       oms.erase(it);
     }
     
     return 0;
   }
*/