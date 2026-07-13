#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <iostream>
#include <math.h>
class Vector2D {
  double x_{0.0}, y_{0.0};

public:
  Vector2D() = default;
  Vector2D(double x, double y);
  double get_x() const;
  double get_y() const;
  double scalar() const;
  double scalar_sq() const;
  Vector2D operator+(const Vector2D &other) const;
  Vector2D operator*(const double x) const;
  Vector2D operator*(const Vector2D &other) const;
  Vector2D operator-(const Vector2D &other) const;
  friend std::ostream &operator<<(std::ostream &os, const Vector2D &v);
};

#endif
