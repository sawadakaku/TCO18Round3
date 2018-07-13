// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <cmath>
#include <utility>

using namespace std;

const int MAX_INVEST = 400000;

unsigned int randxor()
{
  static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
  unsigned int t;
  t = (x^(x<<11));
  x = y;
  y = z;
  z = w;
  return w = (w^(w>>19))^(t^(t>>8));
}

vector<double> softmax(vector<double> x) {
  vector<double> y(x.size());
  double m = 0;
  for (int i = 0; i < x.size(); i++) {
    y[i] = exp(x[i]);
    m += y[i];
  } 
  for (auto&& y_ : y) {
    y_ = y_/m;
  }
  return y;
}

vector<double> linearmax(vector<double> x) {
  vector<double> y(x.size());
  double mi = 0;
  double ma = 2;
  double m = 0;
  for (int i = 0; i < x.size(); i++) {
    y[i] = x[i];
    ma = max(ma, y[i]);
    mi = min(mi, y[i]);
    m += y[i];
  } 
  for (auto&& y_ : y) {
    y_ = (y_ - mi)/(m - static_cast<double>(y.size() * mi));
  }
  return y;
}

class InvestmentAdvice {
public:
  vector<int> getInvestments(vector<int> advice, vector<int> recent, int money, int timeLeft, int roundsLeft) {
    int N = static_cast<int>(advice.size());
    vector<int> ret(N);
    vector<double> x(N), w(N);
    for (int i = 0; i < N; i++) {
      if (advice[i] > 0) {
        x[i] = static_cast<double>(advice[i]%100) * 0.01;
      } else {
        x[i] = 0.0;
      }
    }
    w = linearmax(x);
    double t = 0.0;
    for (auto&& y : w) {
      t += y;
    }
    if (t > 1.0) cerr << "over amount, t = " << t << endl;

    for (int i = 0; i < N; i++) {
      ret[i] = min(MAX_INVEST, static_cast<int>(static_cast<double>(money)*w[i]));
    }
    long long am = 0;
    for (auto&& r : ret) {
      am += r;
    }
    cerr << "money: " << money << ", inv_amt: " << am << ", t: " << t << endl;

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
