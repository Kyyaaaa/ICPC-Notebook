#include <bits/stdc++.h>
using namespace std;
#define bit(i, x) (x >> i & 1)
#define ll long long
const int N = 2e5 + 5;
const int mod = 998244353;

int findFirstDifferentLine(string file1, string file2) {
  ifstream a(file1), b(file2);
  string x, y;
  int line = 1;

  while(true) {
    bool ok1 = (bool)getline(a, x);
    bool ok2 = (bool)getline(b, y);

    if(!ok1 && !ok2) return -1;
    if(ok1 != ok2 || x != y) return line;
    line++;
  }
}

signed main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(0);
  cin.tie(0); cout.tie(0);

  string sol = "main";
  string brute = "brute";
  string gen = "gen";

  for(string name : {sol, brute, gen}) {
    if(system(("./com.sh " + name).c_str()) != 0) {
      cout << "Compilation failed: " << name << '\n';
      return 1;
    }
  }

  int test = 20;
  for(int i = 1; i <= test; i++) {
    if(system(("./" + gen + " > TestInput").c_str()) != 0 ||
       system(("./" + sol + " < TestInput > MainSolution_Output").c_str()) != 0 ||
       system(("./" + brute + " < TestInput > BruteSolution_Output").c_str()) != 0) {
      cout << "Runtime error at test " << i << '\n';
      return 1;
    }

    int val = system("diff -q MainSolution_Output BruteSolution_Output > /dev/null");

    cout << "Test " << i << ": ";
    if(val == 0) {
      cout << "true";
    }
    else {
      cout << "FALSE\n";
      cout << "WRONG ANSWER, FAILED AT TEST CASE " << i << '\n';
      int line = findFirstDifferentLine("MainSolution_Output", "BruteSolution_Output");
      cout << "First different at line " << line << '\n';
      system("diff -u MainSolution_Output BruteSolution_Output");
      return 0;
    }
    cout << '\n';
  }

  cout << "Passed! All test cases are correct!\n";

  return 0 ^ 0;
}
