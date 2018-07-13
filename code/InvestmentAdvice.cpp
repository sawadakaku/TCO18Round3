// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

class InvestmentAdvice {
public:
  vector<int> getInvestments(vector<int> advice, vector<int> recent, int money, int timeLeft, int roundsLeft) {
    vector<int> ret(advice.size());
    for (int i = 0; i < advice.size(); i++) {
      if (advice[i] > 0) ret[i] = 100;
    }
    return ret;
  }
};

// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

int main() {
    InvestmentAdvice sol;
	int roundsLeft = 99;
	while (roundsLeft > 1) {
      int A;
      cin >> A;
      vector<int> advice(A);
      getVector(advice);
      int R;
      cin >> R;
      vector<int> recent(R);
      getVector(recent);
      int money;
	  int timeLeft;
	  cin >> money;
	  cin >> timeLeft;
	  cin >> roundsLeft;
      vector<int> ret = sol.getInvestments(advice, recent, money, timeLeft, roundsLeft);
      cout << ret.size() << endl;
      for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
      cout.flush();
	}
}
