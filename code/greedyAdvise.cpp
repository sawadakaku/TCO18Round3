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

template <typename Sequence, typename BinaryPredicate>
struct IndexCompareT {
  IndexCompareT(const Sequence& seq, const BinaryPredicate comp)
    : seq_(seq), comp_(comp) { }
  bool operator()(const size_t a, const size_t b) const
  {
    return comp_(seq_[a], seq_[b]);
  }
  const Sequence seq_;
  const BinaryPredicate comp_;
};

template <typename Sequence, typename BinaryPredicate>
IndexCompareT<Sequence, BinaryPredicate>
IndexCompare(const Sequence& seq, const BinaryPredicate comp)
{
  return IndexCompareT<Sequence, BinaryPredicate>(seq, comp);
}

template <typename Sequence, typename BinaryPredicate>
std::vector<size_t> ArgSort(const Sequence& seq, BinaryPredicate func)
{
  std::vector<size_t> index(seq.size());
  for (int i = 0; i < index.size(); i++)
    index[i] = i;

  std::sort(index.begin(), index.end(), IndexCompare(seq, func));

  return index;
}

class InvestmentAdvice {
public:
  vector<int> getInvestments(vector<int> advice, vector<int> recent, int money, int timeLeft, int roundsLeft) {
    int N = static_cast<int>(advice.size());
    int rest_money = money;
    vector<int> ret(N);
    vector<double> x(N), w(N);
    for (int i = 0; i < N; i++) {
      if (advice[i] > 0) {
        x[i] = static_cast<double>(advice[i]%100) * 0.01;
      } else {
        x[i] = 0.0;
      }
    }
    std::vector<size_t> sorted_index = ArgSort(x, std::greater<double>());
    for (int i = 0; i < N; i++) {
      if (x[sorted_index[i]] > 0) {
        ret[sorted_index[i]] += min(MAX_INVEST, rest_money);
        rest_money -= min(MAX_INVEST, rest_money);
      }
    }
    long long am = 0;
    int invn = 0;
    for (auto&& r : ret) {
      am += r;
      if (r > 100) invn++;
    }
    cerr << "money: " << money << ", inv_amt: " << am << "inv_n: " << invn << endl;
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
