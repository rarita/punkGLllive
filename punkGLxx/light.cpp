#include "pch.h"
#include "light.h"
#include "nd_vector.h"
using namespace std;

Light::Light(double intensity, Vector3D position) : m_intensity(intensity), m_position(position) {}

double Light::getIntensity() {
	return m_intensity;
}

Vector3D Light::getPosition() {
	return m_position;
}

double const Light::calculateReflections(Vector3D &normal, Vector3D &view, Vector3D &vec_l, double specular) {
	double intensity = 0.0;
	double length_normal = normal.length();
	double length_view = view.length();
	// Diffuse
	double n_dot_l = normal.dotProduct(vec_l);
	if (n_dot_l > 0) {
		intensity += this->getIntensity() * n_dot_l / (length_normal * vec_l.length());
	}
	// Specular
	if (specular > 0) {
		Vector3D vec_r = (normal * (normal.dotProduct(vec_l) * 2.0)) - vec_l;
		double r_dot_v = vec_r.dotProduct(view);
		if (r_dot_v > 0) {
			intensity += this->getIntensity() * pow(r_dot_v / (vec_r.length() * length_view), specular);
		}
	}
	return intensity;
}