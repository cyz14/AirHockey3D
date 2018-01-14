#include "Vector2d.h"
#include <cmath>


Vector2d::Vector2d()
	: x(0), y(0)
{
}

Vector2d::Vector2d(double x, double y)
	: x(x), y(y)
{}

Vector2d::Vector2d(const Vector2d& v)
	:x(v.x), y(v.y)
{
}

Vector2d::~Vector2d()
{
}

double Vector2d::norm() {
	return sqrt(x*x + y * y);
}

void Vector2d::normalize() {
	double length = norm();
	x /= length;
	y /= length;
}

double Vector2d::sqrNorm() {
	return x * x + y * y;
}

double Vector2d::dot(Vector2d op) {
	return x * op.x + y * op.y;
}

