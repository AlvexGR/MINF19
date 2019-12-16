#include "stdc++.h"
using namespace std;
int main() {
  int i;
  vector<int> a;
  for (int i = 0; i < 10; i++) {
    a.push_back(i);
  }
  default_random_engine randomGenerator;
  set<string> uniqueStr;
  for (int i = 0; i < 200; i++) {
    shuffle(a.begin(), a.end(), randomGenerator);
    string s = "";
    for (auto it : a) {
      s += (it + '0');
    }
    cout << s << endl;
    uniqueStr.insert(s);
  }
  cout << "Diff " << uniqueStr.size() << endl;
  return 0;
}
