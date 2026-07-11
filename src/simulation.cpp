#include <chrono> // Подключаем библиотеку для работы со времене
#include <cmath>
#include <iostream>
#include <math.h>
#include <thread> // Подключаем библиотеку для работы с потоками
#include <utility>

using pd = std::pair<double, double>;
const double G = 1e-8;
const double T = 10e-5;
const double Eps = 10e-10;

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
int is_equal(double a, double b) {
  if (fabs(a - b) < Eps)
    return 1;
  return 0;
}

class Vector2D {
  double x_{0.0}, y_{0.0};

public:
  Vector2D() = default;
  Vector2D(double x, double y) : x_{x}, y_{y} {}
  double get_x() { return this->x_; }
  double get_y() { return this->y_; }
  Vector2D operator+(const Vector2D &other) {
    return Vector2D(this->x_ + other.x_, this->y_ + other.y_);
  }
  Vector2D operator*(const double x) {
    return Vector2D(x * this->x_, x * this->y_);
  }
  Vector2D operator*(const Vector2D &other) {
    return Vector2D(this->x_ * other.x_, this->y_ * other.y_);
  }
  Vector2D operator-(const Vector2D &other) {
    return Vector2D(this->x_ - other.x_, this->y_ - other.y_);
  }
  friend std::ostream &operator<<(std::ostream &os, const Vector2D &v);
};

std::ostream &operator<<(std::ostream &os, const Vector2D &v) {
  os << "(" << v.x_ << ", " << v.y_ << ")";
  return os; // Returns stream reference to allow chaining (e.g., cout << v1 <<
             // v2;)
}

class Object {
  Vector2D coord_{};
  double mass_{1.0};
  Vector2D velocity_{};

public:
  Object() = default;
  Object(double x, double y, double mass)
      : coord_(x, y), mass_{mass}, velocity_{} {}
  Object(double x, double y, double mass, Vector2D v)
      : coord_(x, y), mass_{mass}, velocity_{v} {}
  double get_x() { return coord_.get_x(); }
  double get_y() { return coord_.get_y(); }
  double get_vx() { return velocity_.get_x(); }
  double get_vy() { return velocity_.get_y(); }
  friend double distance_squared(Object &o1, Object &o2);
  friend Vector2D norm_vec(Object &o1, Object &o2);
  friend Vector2D grav_force(Object &o1, Object &o2);
  friend void apply_force(Object &o, Vector2D force);
  friend void tic_step(Object &o);
};
double distance_squared(Object &o1, Object &o2) {
  return (pow(o1.get_x() - o2.get_x(), 2) + pow(o1.get_y() - o2.get_y(), 2));
}
Vector2D norm_vec(Object &o1,
                  Object &o2) { // normalized vector from o1 that directs to o2
  Vector2D n = o2.coord_ - o1.coord_;
  return n;
}
Vector2D grav_force(Object &o1, Object &o2) {
  return norm_vec(o1, o2) *
         (G * (o1.mass_ * o2.mass_) / (distance_squared(o1, o2) + Eps));
}
void apply_force(Object &o, Vector2D force) {
  o.velocity_ = o.velocity_ + force * (T / o.mass_);
}
void tic_step(Object &o) { o.coord_ = o.coord_ + o.velocity_ * T; }

int main() {
  Object o1{0.0, 0.0, 2.0}, o2{1.0, 0.0, 1.0};
  std::cout << norm_vec(o1, o2) << " " << distance_squared(o1, o2) << " "
            << grav_force(o1, o2) << std::endl;
  double t = 0;
  long long step = 0;
  const long long steps_per_second = (long long)(1.0 / T + 0.5);

  while (true) {
    apply_force(o1, grav_force(o1, o2));
    apply_force(o2, grav_force(o2, o1));
    tic_step(o1);
    tic_step(o2);
    step++;
    t = T * step;
    if (step % steps_per_second == 0) {
      std::cout << "Time: " << t << std::endl;
      // std::cout << " Vel 1: " << o1.get_vx() << " " << o1.get_vy()
      //           << "| Vel 2: " << o2.get_vx() << " " << o2.get_vy()
      //           << std::endl;
      std::cout << " (1) : (" << o1.get_x() << " " << o1.get_y() << ")| (2) : ("
                << o2.get_x() << " " << o2.get_y() << ")" << std::endl;
      // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  }
}
