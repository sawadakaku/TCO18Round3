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
#include <functional>

using namespace std;

const int MAX_INVEST = 400000;

struct Hist {
  double ad;
  double rt;
};

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

double gaussian(double x, double mu, double sig) {
  return exp(-pow(x-mu, 2)/(2*pow(sig, 2)))/(sig*sqrt(2*M_PI));
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
private:
  vector<double> accs, stdevs;
  vector< vector<Hist> > hists;
  vector<int> lastinvests;
  vector<int> lastadvices;
  int N;
  int cnt = 0;
  void init(int N) {
    this->N = N;
    for (int i = 0; i < N; i++) {
      accs.push_back(0.5);
      stdevs.push_back(0.1);
      hists.push_back(vector<Hist>());
      lastinvests.push_back(0);
      lastadvices.push_back(0);
    }
  }
  double calc_likelihood(int i, double acc, double stdev) {
    double ret = 1.0;
    for (int j = 0; j < this->hists[i].size(); j++) {
      double llh;
      llh = (1-acc) * gaussian(this->hists[i][j].ad, 0.0, 0.1);
      llh += acc * gaussian(this->hists[i][j].ad, this->hists[i][j].rt, stdev);
      ret *= llh;
    }
    return ret;
  }
  void sa(int i) {
    int num_loop = 10;
    double best_acc, best_stdev, curr_acc, curr_stdev, best_llh, curr_llh;
    best_acc = this->accs[i];
    best_stdev = this->stdevs[i];
    curr_acc = best_acc;
    curr_stdev = curr_stdev;
    best_llh = this->calc_likelihood(i, best_acc, best_stdev);
    for (int j = 0; j < num_loop; j++) {
      curr_acc = min(1.0, best_acc + 0.01);
      curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
      if (best_llh < curr_llh) {
        best_acc = curr_acc;
        best_llh = curr_llh;
      } else {
        curr_acc = max(0.0, best_acc - 0.01);
        curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
        if (best_llh < curr_llh) {
          best_acc = curr_acc;
          best_llh = curr_llh;
        }
      }
      curr_stdev = min(0.2, best_stdev + 0.002);
      curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
      if (best_llh < curr_llh) {
        best_stdev = curr_stdev;
        best_llh = curr_llh;
      } else {
        curr_stdev = max(0.0, best_stdev - 0.002);
        curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
        if (best_llh < curr_llh) {
          best_stdev = curr_stdev;
          best_llh = curr_llh;
        }
      }
    }
    for (int j = 0; j < num_loop; j++) {
      curr_acc = min(1.0, best_acc + 0.001);
      curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
      if (best_llh < curr_llh) {
        best_acc = curr_acc;
        best_llh = curr_llh;
      } else {
        curr_acc = max(0.0, best_acc - 0.001);
        curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
        if (best_llh < curr_llh) {
          best_acc = curr_acc;
          best_llh = curr_llh;
        }
      }
      curr_stdev = min(0.2, best_acc + 0.00002);
      curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
      if (best_llh < curr_llh) {
        best_stdev = curr_stdev;
        best_llh = curr_llh;
      } else {
        curr_stdev = max(0.0, best_stdev - 0.0002);
        curr_llh = this->calc_likelihood(i, curr_acc, curr_stdev);
        if (best_llh < curr_llh) {
          best_stdev = curr_stdev;
          best_llh = curr_llh;
        }
      }
    }
    this->accs[i] = best_acc;
    this->stdevs[i] = best_stdev;
  }
  void update_params() {
    for (int i = 0; i < this->N; i++) {
      this->sa(i);
    }
  }
  void update(vector<int> recent) {
    for (int i = 0; i < this->N; i++) {
      Hist t = {static_cast<double>(lastadvices[i])/100.0,
                static_cast<double>(recent[i])/static_cast<double>(lastinvests[i])};
      this->hists[i].push_back(t);
    }
    this->update_params();
  }
  double calc_expectation(int i, int advice) {
    double ret = 0.0;
    double ad = static_cast<double>(advice)*0.01;
    double acc = this->accs[i];
    double stdev = this->stdevs[i];
    for (int j = 0; j <= 200; j++) {
      double t;
      double x = -1.0 + static_cast<double>(j)*0.01;
      t = (1-acc) * gaussian(x, 0.0, 0.1);
      t += acc * gaussian(x, ad, stdev);
      ret += t * 0.005 * x;
    }
    return ret;
  }
  vector<double> calc_expectations(vector<int> advice) {
    vector<double> ret;
    for (int i = 0; i < this->N; i++) {
      ret.push_back(calc_expectation(i, advice[i]));
    }
    return ret;
  }
public:
  vector<int> getInvestments(vector<int> advice, vector<int> recent, int money, int timeLeft, int roundsLeft) {
    int N = static_cast<int>(advice.size());
    int rest_money = money - N*100;
    vector<int> ret(N, 100);
    vector<double> x(N), w(N);
    if (this->cnt == 0) {
      this->init(N);
    } else {
      this->update(recent);
    }
    x = this->calc_expectations(advice);
    /*
    for (int i = 0; i < N; i++) cerr << x[i] << ' ';
    cerr << endl;
    */
    for (int i = 0; i < N; i++) {
      x[i] = max(x[i], 0.0);
    }
    /*
    for (int i = 0; i < N; i++) cerr << x[i] << ' ';
    cerr << endl;
    */

    std::vector<size_t> sorted_index = ArgSort(x, std::greater<double>());
    for (int i = 0; i < N; i++) {
      if (x[sorted_index[i]] > 0) {
        ret[sorted_index[i]] += min(MAX_INVEST-100, rest_money);
        rest_money -= min(MAX_INVEST-100, rest_money);
      }
    }
    long long am = 0;
    int invn = 0;
    for (auto&& r : ret) {
      am += r;
      if (r > 100) invn++;
    }
    cerr << "money: " << money << ", inv_amt: " << am << "inv_n: " << invn << endl;
    this->cnt++;
    if (roundsLeft == 1) {
      for (int i = 0; i < N; i++) {
        cerr << "Expert" << i << ": acc=" << this->accs[i] << ", stdev=" << this->stdevs[i] << endl;
      }
    }
    this->lastadvices = advice;
    this->lastinvests = ret;
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
