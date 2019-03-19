#pragma once
#include "nd_vector.h"
#include "color.h"

class Sphere {
public:
	Sphere(Vector3D center, double radius, RGB color, double specular);
	Vector2D const intersectWithRay(Vector3D &origin, Vector3D &direction);
	Vector3D const getCenter();
	RGB const getColor();
	double const getRadius();
	double const getSpecular();
	string toString();
private:
	const Vector3D m_center;
	const double m_radius;
	const RGB m_color;
	const double m_specular;
};