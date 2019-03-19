#pragma once
#include <vector>
#include <string>


class Vector2D {
public:
	Vector2D(double x, double y);
	double const getX();
	double const getY();
	std::string const toString();
private:
	const double m_x;
	const double m_y;
};

class Vector3D : Vector2D {
public:
	Vector3D(double x, double y, double z);
	double const dotProduct(Vector3D other);
	double const length();
	Vector3D operator *(Vector3D &other); // Product as operator
	Vector3D operator *(double multiplier);
	Vector3D operator +(Vector3D &other);
	Vector3D operator -(Vector3D &other); // subtract as operator
	Vector3D clamp();
	double const getZ();
	std::string const toString();
private:
	const double m_z;
};