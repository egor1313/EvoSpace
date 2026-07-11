#include <chrono> // Подключаем библиотеку для работы со времене
#include <cmath>
#include <iostream>
#include <math.h>
#include <thread> // Подключаем библиотеку для работы с потоками
#include <utility>
#include <vector>

using pd = std::pair<double, double>;
const double G = 1e-8;
const double T = 1e-5;
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
  double get_x() const { return this->x_; }
  double get_y() const { return this->y_; }
  double scalar() const { return std::sqrt(pow(x_, 2) + pow(y_, 2)); }
  double scalar_sq() const { return (pow(x_, 2) + pow(y_, 2)); }
  Vector2D operator+(const Vector2D &other) const {
    return Vector2D(this->x_ + other.x_, this->y_ + other.y_);
  }
  Vector2D operator*(const double x) const {
    return Vector2D(x * this->x_, x * this->y_);
  }
  Vector2D operator*(const Vector2D &other) const {
    return Vector2D(this->x_ * other.x_, this->y_ * other.y_);
  }
  Vector2D operator-(const Vector2D &other) const {
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
  double get_x() const { return coord_.get_x(); }
  double get_y() const { return coord_.get_y(); }
  double get_vx() const { return velocity_.get_x(); }
  double get_vy() const { return velocity_.get_y(); }
  double scalar_velocity_sq() const { return velocity_.scalar_sq(); }
  double energy_k() const { return 0.5 * mass_ * scalar_velocity_sq(); }
  Vector2D impulse() const { return velocity_ * mass_; }
  friend double distance_squared(const Object &o1, const Object &o2);
  friend Vector2D norm_vec(const Object &o1, const Object &o2);
  friend Vector2D grav_force(const Object &o1, const Object &o2);
  friend void apply_force(Object &o, Vector2D force);
  friend void tic_step(Object &o);
  friend const double energy_p2(const Object &o1, const Object &o2);
};
double distance_squared(const Object &o1, const Object &o2) {
  return (pow(o1.get_x() - o2.get_x(), 2) + pow(o1.get_y() - o2.get_y(), 2));
}
Vector2D
norm_vec(const Object &o1,
         const Object &o2) { // normalized vector from o1 that directs to o2
  Vector2D n = o2.coord_ - o1.coord_;
  return n;
}
Vector2D grav_force(const Object &o1, const Object &o2) {
  double dist = distance_squared(o1, o2);
  if (dist < Eps)
    return norm_vec(o1, o2) *
           (G * (o1.mass_ * o2.mass_) / (distance_squared(o1, o2) + Eps));
  else
    return norm_vec(o1, o2) *
           (G * (o1.mass_ * o2.mass_) / (distance_squared(o1, o2)));
}
void apply_force(Object &o, Vector2D force) {
  o.velocity_ = o.velocity_ + force * (T / o.mass_);
}
void tic_step(Object &o) { o.coord_ = o.coord_ + o.velocity_ * T; }

const double energy_p2(const Object &o1, const Object &o2) {
  double dist = std::sqrt(distance_squared(o1, o2));
  return (-G * (o1.mass_ * o2.mass_)) / ((dist < Eps) ? dist + Eps : dist);
}

Vector2D compute_total_momentum(const std::vector<Object> &bodies) {
  Vector2D s{};
  for (Object a : bodies) {
    s = s + a.impulse();
  }
  return s;
}

float compute_total_energy(const std::vector<Object> &b) {
  float T = 0.0;
  float U = 0.0;
  int N = b.size();
  for (const auto &p : b) {
    T += p.energy_k();
  }

  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      U += energy_p2(b[i], b[j]);
    }
  }
  return T + U;
}
int main() {
  Object o1{0.0, 0.0, 1000.0}, o3{10.0, 0, 1.0, Vector2D{0, 0}};
  std::vector<Object> bodies = {o1, o3};
  int size_of_bodies = 2;
  double t = 0;
  long long step = 0;
  const long long steps_per_second = (long long)(1.0 / T + 0.5);

  while (true) {
    for (int i = 0; i < size_of_bodies; i++) {
      for (int j = 0; j < size_of_bodies; j++) {
        if (i == j)
          continue;
        apply_force(bodies[i], grav_force(bodies[i], bodies[j]));
      }
    }

    for (int i = 0; i < size_of_bodies; i++) {
      tic_step(bodies[i]);
    }
    step++;
    t = T * step;
    if (step == 1 || (step % (steps_per_second * 100) == 0)) {
      Vector2D M = compute_total_momentum(bodies);
      float E = compute_total_energy(bodies);
      std::cout << "Time: " << t << std::endl;
      // std::cout << " Vel 1: " << o1.get_vx() << " " << o1.get_vy()
      //           << "| Vel 2: " << o2.get_vx() << " " << o2.get_vy()
      //           << std::endl;
      std::cout << "Full energy: " << E << " | Full impulse: " << M
                << std::endl;
      // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    if (step == 100000 * steps_per_second)
      return 0;
  }
}
