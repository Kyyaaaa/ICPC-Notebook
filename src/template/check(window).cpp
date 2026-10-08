#include <bits/stdc++.h>
using namespace std;
#define bit(i, x) (x >> i & 1)
#define ll long long
const int N = 2e5 + 5;
const int mod = 998244353;

int findFirstDifferentLine(string file1, string file2) {
  ifstream a(file1, ios::binary), b(file2, ios::binary);
  int line = 1;

  while(true) {
    int x = a.get(), y = b.get();
    if(x != y) return line;
    if(x == EOF) return -1;
    if(x == '\n') line++;
  }
}

signed main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(0);
  cin.tie(0); cout.tie(0);

  string sol = "main";
  string brute = "brute";
  string gen = "gen";

  for(string name : {sol, brute, gen}) {
    if(system(("call com.bat " + name).c_str()) != 0) {
      cout << "Compilation failed: " << name << '\n';
      return 1;
    }
  }

  int test = 20;
  if(argc > 1) test = atoi(argv[1]);

  for(int i = 1; i <= test; i++) {
    if(system((gen + ".exe > TestInput").c_str()) != 0 ||
       system((sol + ".exe < TestInput > MainSolution_Output").c_str()) != 0 ||
       system((brute + ".exe < TestInput > BruteSolution_Output").c_str()) != 0) {
      cout << "Runtime error at test " << i << '\n';
      return 1;
    }

    int val = system("fc /b MainSolution_Output BruteSolution_Output > nul");

    cout << "Test " << i << ": ";
    if(val == 0) {
      cout << "true";
    }
    else {
      if(val != 1) {
        cout << "File comparison failed\n";
        return 1;
      }
      cout << "FALSE\n";
      cout << "WRONG ANSWER, FAILED AT TEST CASE " << i << '\n';
      int line = findFirstDifferentLine("MainSolution_Output", "BruteSolution_Output");
      cout << "First different at line " << line << '\n';
      system("fc MainSolution_Output BruteSolution_Output");
      return 1;
    }
    cout << '\n';
  }

  cout << "Passed! All test cases are correct!\n";
  return 0;
}
/*
com check
check
check 1000
*/