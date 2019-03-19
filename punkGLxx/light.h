#pragma once
#include "nd_vector.h"


class Light {
public:
	Light(double intensity, Vector3D position);
	double const calculateReflections(Vector3D &normal, Vector3D &view, Vector3D &vec_l, double specular);
	double getIntensity();
	Vector3D getPosition();
private:
	const double m_intensity;
	const Vector3D m_position;
};