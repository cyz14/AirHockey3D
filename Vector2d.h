#pragma once
class Vector2d
{
	double x, y;
public:
	Vector2d();
	Vector2d(double x, double y);
	Vector2d(const Vector2d& v);
	double norm();
	void normalize();
	double sqrNorm();
	double dot(Vector2d op);
	double getX() { return x; }
	double getY() { return y; }
	friend Vector2d operator + (const Vector2d& v1, const Vector2d& v2) { return Vector2d(v1.x + v2.x, v1.y + v2.y); }
	friend Vector2d operator - (const Vector2d& v1, const Vector2d& v2) { return Vector2d(v1.x - v2.x, v1.y - v2.y); }
	friend Vector2d operator * (const Vector2d& v1, const double& d) { return Vector2d(v1.x * d, v1.y * d); }
	friend Vector2d operator / (const Vector2d& v1, const double& d) { return Vector2d(v1.x / d, v1.y / d); }
	friend Vector2d operator * (const double& d,    const Vector2d& v1) { return Vector2d(v1.x * d, v1.y * d); }
	~Vector2d();
};

