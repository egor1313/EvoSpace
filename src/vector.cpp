#include "vector.h"

Vector2D::Vector2D(double x, double y) : x_{x}, y_{y} {}
double Vector2D::get_x() const { return this->x_; }
double Vector2D::get_y() const { return this->y_; }
double Vector2D::scalar() const { return std::sqrt(x_ * x_ + y_ * y_); }
double Vector2D::scalar_sq() const { return (x_ * x_ + y_ * y_); }
Vector2D Vector2D::operator+(const Vector2D &other) const {
  return Vector2D(this->x_ + other.x_, this->y_ + other.y_);
}
Vector2D Vector2D::operator*(const double x) const {
  return Vector2D(x * this->x_, x * this->y_);
}
Vector2D Vector2D::operator*(const Vector2D &other) const {
  return Vector2D(this->x_ * other.x_, this->y_ * other.y_);
}
Vector2D Vector2D::operator-(const Vector2D &other) const {
  return Vector2D(this->x_ - other.x_, this->y_ - other.y_);
}

std::ostream &operator<<(std::ostream &os, const Vector2D &v) {
  os << "(" << v.x_ << ", " << v.y_ << ")";
  return os;
}
