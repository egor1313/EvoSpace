#include <iostream>
#include <math.h>
#include <utility>

using pd = std::pair<double, double>;

double pow(double a, int n) {
  if (n == 0)
    return 1;
  if (n == 1)
    return a;
  if (n % 2 == 0)
    return pow(a, n / 2) * pow(a, n / 2);
  else
    return a * pow(a, n - 1);
}

class Vector {};

class Object {
  double x_{0.0}, y_{0.0};
  double mass_{1.0};
  pd velocity_{};

public:
  Object() = default;
  Object(double x, double y, double mass)
      : x_{x}, y_{y}, mass_{mass}, velocity_{} {}
  Object(double x, double y, double mass, pd v)
      : x_{x}, y_{y}, mass_{mass}, velocity_{v} {}
  friend double distance(Object o1, Object o2);
};
double distance(Object o1, Object o2) {
  return std::sqrt(pow(o1.x_ - o2.x_, 2) + pow(o1.y_ - o2.y_, 2));
}
pd norm_vec(Object o1,
            Object o2) { // normalized vector from o1 that directs to o2
  pd n;
}

int main() {
  Object o1{0.0, 0.0, 2.0}, o2{1.0, 0.0, 1.0};

  while (true) {
  }
}
