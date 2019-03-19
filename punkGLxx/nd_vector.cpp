#include "pch.h"
#include <string>
#include "nd_vector.h"

using namespace std;
// Vector2D
Vector2D::Vector2D(double x, double y) : m_x(x), m_y(y) {}

double const Vector2D::getX() {
	return m_x;
}

double const Vector2D::getY() {
	return m_y;
}

string const Vector2D::toString() {
	return "Vector3D(x = " + to_string(this->getX()) +
		";y = " + to_string(this->getY()) + ")";
}

// Vector3D
Vector3D::Vector3D(double x, double y, double z) : Vector2D(x, y), m_z(z) {}

double const Vector3D::getZ() {
	return m_z;
}

double const Vector3D::dotProduct(Vector3D other) {
	return
		this->getX() * other.getX() +
		this->getY() * other.getY() +
		this->getZ() * other.getZ();
}

double const Vector3D::length() {
	return sqrt(this->dotProduct(*this));
}

Vector3D Vector3D::operator*(Vector3D &other) {
	return Vector3D(
		this->getX() * other.getX(),
		this->getY() * other.getY(),
		this->getZ() * other.getZ()
	);
}

Vector3D Vector3D::operator*(double multiplier) {
	return Vector3D(
		this->getX() * multiplier,
		this->getY() * multiplier,
		this->getZ() * multiplier
	);
}

Vector3D Vector3D::operator+(Vector3D &other) {
	return Vector3D(
		this->getX() + other.getX(),
		this->getY() + other.getY(),
		this->getZ() + other.getZ()
	);
}

Vector3D Vector3D::operator-(Vector3D &other) { // Можно это переписать как this + other * (-1), но как-то похуй.
	return Vector3D(
		this->getX() - other.getX(),
		this->getY() - other.getY(),
		this->getZ() - other.getZ()
	);
}

const string Vector3D::toString() {
	return "Vector3D(x = " + to_string(this->getX()) +
		";y = " + to_string(this->getY()) +
		";z = " + to_string(this->getZ()) + ";)";
}
