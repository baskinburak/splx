#include <qpOASES.hpp>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <random>
#include <iostream>
#include "bspline.h"
#include <chrono>

using std::cout;
using std::endl;

int main() {
  std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<double> distribution(-10, 10);
  auto rand = std::bind(distribution, generator);

  std::vector<splx::Vec> cpts;

  cpts.resize(20);

  for(size_t i = 0; i < cpts.size(); i++) {
    splx::Vec v(2);
    v(0) = i * 0.2;
    v(1) = (i+rand()) * 0.2;
    cpts[i] = v;
    cout << "c" << endl;
    cout << v << endl;
  }

  splx::BSpline bspl(2, 2, 0, 1.0, cpts);

  int i = 0;

  for(double t = 0; t<=1.0; t+=0.001) {
    cout << "d" << endl;
    cout << bspl.eval(t, 0) << endl;
    cout << "d2" << endl;
    cout << bspl.eval_dbg(t) << endl;
    if((i++)%50 == 0) {
      cout << "v" << endl;
      cout << bspl.eval(t, 1) << endl;
    }
  }
  cout << "d" << endl;
  cout << bspl.eval(1.0, 0) << endl;
  cout << "d2" << endl;
  cout << bspl.eval_dbg(1.0) << endl;
  cout << "v" << endl;
  cout << bspl.eval(1.0, 1) << endl;

  splx::Matrix H = bspl.getZeroHessian();
  splx::Matrix g = bspl.getZeroG();

  bspl.extendQPIntegratedSquaredDerivative(H, 2, 0.1);

  return 0;
}
