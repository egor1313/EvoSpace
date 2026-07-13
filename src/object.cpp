#include "object.h"

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

Object::Object(double x, double y, double mass)
    : coord_(x, y), mass_{mass}, velocity_{} {}
Object::Object(double x, double y, double mass, Vector2D v)
    : coord_(x, y), mass_{mass}, velocity_{v} {}
double Object::get_mass() const { return mass_; }
double Object::get_x() const { return coord_.get_x(); }
double Object::get_y() const { return coord_.get_y(); }
double Object::get_vx() const { return velocity_.get_x(); }
double Object::get_vy() const { return velocity_.get_y(); }
double Object::scalar_velocity_sq() const { return velocity_.scalar_sq(); }
double Object::energy_k() const { return 0.5 * mass_ * scalar_velocity_sq(); }
Vector2D Object::impulse() const { return velocity_ * mass_; }

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
