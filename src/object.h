#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "vector.h"
#include <cmath>
#include <iostream>
#include <math.h>

////////// <- MAIN CONSTANTS -> ///////////////
const double G = 1e-8;
const double T = 1e-2;
const double Eps = 10e-10;

class Object {
protected:
  Vector2D coord_{};
  double mass_{1.0};
  Vector2D velocity_{};

public:
  Object() = default;
  Object(double x, double y, double mass);
  Object(double x, double y, double mass, Vector2D v);
  double get_mass() const;
  double get_x() const;
  double get_y() const;
  double get_vx() const;
  double get_vy() const;
  double scalar_velocity_sq() const;
  double energy_k() const;
  Vector2D impulse() const;
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

#endif
