#include "pch.h"
#include "primitives.h"
#include "nd_vector.h"
#include "color.h"

using namespace std;

Sphere::Sphere(const Vector3D center, double radius, RGB color, double specular)
	: m_center(center), m_radius(radius), m_color(color), m_specular(specular) {}

Vector3D const Sphere::getCenter() {
	return m_center;
}

RGB const Sphere::getColor() {
	return m_color;
}

double const Sphere::getRadius() {
	return m_radius;
}

double const Sphere::getSpecular() {
	return m_specular;
}

Vector2D const Sphere::intersectWithRay(Vector3D &origin, Vector3D &direction) {
	Vector3D center = this->getCenter();
	Vector3D oc = origin - center;

	const double k1 = direction.dotProduct(direction);
	const double k2 = 2 * oc.dotProduct(direction);
	const double k3 = oc.dotProduct(oc) - pow(this->getRadius(), 2);

	const double discriminant = pow(k2, 2) - 4 * k1 * k3;
	if (discriminant < 0)
		return Vector2D(numeric_limits<double>::infinity(), numeric_limits<double>::infinity());

	const double plot1 = (-k2 + sqrt(discriminant)) / (2 * k1);
	const double plot2 = (-k2 - sqrt(discriminant)) / (2 * k1);
	return Vector2D(plot1, plot2);
}

string Sphere::toString() {
	RGB m_clr = this->getColor(); Vector3D m_cnt = this->getCenter();
	return "Sphere(center = " + m_cnt.toString()
		+ ";radius = " + to_string(m_radius)
		+ ";color = " + m_clr.toString() + ")";
}