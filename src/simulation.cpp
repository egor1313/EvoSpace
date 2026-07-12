#include <chrono> // Подключаем библиотеку для работы со времене
#include <cmath>
#include <cstdio>  // для snprintf
#include <cstring> // memset
#include <iomanip>
#include <iostream>
#include <math.h>
#include <thread> // Подключаем библиотеку для работы с потоками
#include <utility>
#include <vector>

using pd = std::pair<double, double>;
const double G = 1e-8;
const double T = 1e-2;
const double Eps = 10e-10;
// const long long steps_per_second = 1000;

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
  double get_mass() const { return mass_; }
  double get_x() const { return coord_.get_x(); }
  double get_y() const { return coord_.get_y(); }
  double get_vx() const { return velocity_.get_x(); }
  double get_vy() const { return velocity_.get_y(); }
  double scalar_velocity_sq() const { return velocity_.scalar_sq(); }
  double energy_k() const { return 0.5 * mass_ * scalar_velocity_sq(); }
  Vector2D impulse() const { return velocity_ * mass_; }
  friend double distance_squared(const Object &o1, const Object &o2);
  friend Vector2D delta_vec(const Object &o1, const Object &o2);
  friend Vector2D grav_force(const Object &o1, const Object &o2,
                             double softening);
  friend void verlet_step_first(Object &o, Vector2D &total_force);
  friend void verlet_step_second(Object &o, Vector2D &total_force,
                                 Vector2D &total_force_new);
  friend const double energy_p2(const Object &o1, const Object &o2,
                                double softening);
};
double distance_squared(const Object &o1, const Object &o2) {
  return (pow(o1.get_x() - o2.get_x(), 2) + pow(o1.get_y() - o2.get_y(), 2));
}
Vector2D delta_vec(const Object &o1,
                   const Object &o2) { // vector from o1 that directs to o2
  Vector2D n = o2.coord_ - o1.coord_;
  return n;
}
Vector2D grav_force(const Object &o1, const Object &o2,
                    double softening = 1e-9) {
  Vector2D delta = delta_vec(o1, o2);
  double dist2 = delta.scalar_sq();
  double softened_dist2 = dist2 + softening * softening;
  double inv_dist3 = 1.0 / (softened_dist2 * std::sqrt(softened_dist2));
  return delta * (G * o1.mass_ * o2.mass_ * inv_dist3);
}

void verlet_step_first(Object &o, Vector2D &total_force) {
  Vector2D a = total_force * (1 / o.mass_);
  o.coord_ = o.coord_ + o.velocity_ * T + a * (0.5 * T * T);
}

void verlet_step_second(Object &o, Vector2D &total_force,
                        Vector2D &total_force_new) {
  Vector2D a_new = total_force_new * (1.0 / o.mass_);
  Vector2D a = total_force * (1 / o.mass_);
  o.velocity_ = o.velocity_ + (a + a_new) * (T / 2);
}

const double energy_p2(const Object &o1, const Object &o2,
                       double softening = 1e-9) {
  double r = std::sqrt(distance_squared(o1, o2) + softening * softening);
  return -G * o1.mass_ * o2.mass_ / r;
}

Vector2D compute_total_momentum(const std::vector<Object> &bodies) {
  Vector2D s{};
  for (Object a : bodies) {
    s = s + a.impulse();
  }
  return s;
}

double compute_total_energy(const std::vector<Object> &b) {
  double kin = 0.0;
  double pot = 0.0;
  int N = b.size();
  for (const auto &p : b) {
    kin += p.energy_k();
  }

  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      pot += energy_p2(b[i], b[j]);
    }
  }
  return kin + pot;
}

void clear_screen() { std::cout << "\033[2J\033[H" << std::flush; }

void draw_bodies(const std::vector<Object> &bodies, int screen_width = 80,
                 int screen_height = 24, double world_x_min = -20.0,
                 double world_x_max = 20.0, double world_y_min = -15.0,
                 double world_y_max = 15.0) {
  double scale_x = (world_x_max - world_x_min) / screen_width;
  double scale_y = (world_y_max - world_y_min) / screen_height;

  std::vector<std::string> screen(screen_height,
                                  std::string(screen_width, ' '));

  for (const auto &obj : bodies) {
    int col = static_cast<int>((obj.get_x() - world_x_min) / scale_x);
    int row = static_cast<int>((obj.get_y() - world_y_min) / scale_y);
    row = screen_height - 1 - row;
    if (col >= 0 && col < screen_width && row >= 0 && row < screen_height) {
      char c = (obj.get_mass() > 100.0) ? 'O' : 'o';
      screen[row][col] = c;
    }
  }

  std::string out;
  out.reserve(screen_height * (screen_width + 1));
  for (const auto &line : screen) {
    out += line;
    out += '\n';
  }
  std::cout << out << std::flush;
}

int main() {
  Object o1{0.0, 0.0, 1000.0},
      o3{10.0, 0, 1.0, Vector2D{0, std::sqrt(G * 100)}};
  Object o2{5.0, 5.0, 10.0, Vector2D{std::sqrt(G * 40), 0}};
  Object o4{0.0, -5.0, 1.0, Vector2D{-std::sqrt(G * 200), 0.0}};
  std::vector<Object> bodies = {o1, o2, o3, o4};
  int size_of_bodies = 4;
  double t = 0;
  long long step = 0;
  const long long steps_per_second = (long long)(1.0 / T + 0.05);
  // double En0 = compute_total_energy(bodies);

  /*  while (true) {
      std::vector<Vector2D> forces_old(size_of_bodies),
          forces_new(size_of_bodies);

      for (int i = 0; i < size_of_bodies; ++i) {
        forces_old[i] = {0, 0};
        for (int j = 0; j < size_of_bodies; ++j)
          if (i != j)
            forces_old[i] = forces_old[i] + grav_force(bodies[i], bodies[j]);
      }

      for (int i = 0; i < size_of_bodies; ++i)
        verlet_step_first(bodies[i], forces_old[i]);

      for (int i = 0; i < size_of_bodies; ++i) {
        forces_new[i] = {0, 0};
        for (int j = 0; j < size_of_bodies; ++j)
          if (i != j)
            forces_new[i] = forces_new[i] + grav_force(bodies[i], bodies[j]);
      }

      for (int i = 0; i < size_of_bodies; ++i)
        verlet_step_second(bodies[i], forces_old[i], forces_new[i]);

      step++;
      t = T * step;
      if (step == 1 || (step % (steps_per_second * 100) == 0)) {
        Vector2D M = compute_total_momentum(bodies);
        double E = compute_total_energy(bodies);
        std::cout << "Time: " << t << std::endl;
        std::cout << "Full energy: " << std::scientific << std::setprecision(12)
                  << E << "| (E-E0)/E0: " << (E - En0) / En0
                  << " | Full impulse: " << M << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
      if (step == 100000 * steps_per_second)
        return 0;

    }*/

  // const int fps = 30;
  const long long steps_per_frame = steps_per_second * 1000;
  long long next_frame_step = 0;

  clear_screen();
  while (true) {

    std::vector<Vector2D> forces_old(size_of_bodies),
        forces_new(size_of_bodies);

    for (int i = 0; i < size_of_bodies; ++i) {
      forces_old[i] = {0, 0};
      for (int j = 0; j < size_of_bodies; ++j)
        if (i != j)
          forces_old[i] = forces_old[i] + grav_force(bodies[i], bodies[j]);
    }

    for (int i = 0; i < size_of_bodies; ++i)
      verlet_step_first(bodies[i], forces_old[i]);

    for (int i = 0; i < size_of_bodies; ++i) {
      forces_new[i] = {0, 0};
      for (int j = 0; j < size_of_bodies; ++j)
        if (i != j)
          forces_new[i] = forces_new[i] + grav_force(bodies[i], bodies[j]);
    }

    for (int i = 0; i < size_of_bodies; ++i)
      verlet_step_second(bodies[i], forces_old[i], forces_new[i]);
    step++;
    t = T * step;

    if (step >= next_frame_step) {
      clear_screen();
      draw_bodies(bodies);
      std::cout << "t=" << t << " E=" << compute_total_energy(bodies) << "\n";
      next_frame_step = step + steps_per_frame;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (step == 100000 * steps_per_second)
      break;
  }
}
