#include "pch.h"
#include "color.h"
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
typedef unsigned int uint;

RGB::RGB()
{
	R = 0;
	G = 0;
	B = 0;
}

RGB::RGB(uint r, uint g, uint b) {
	R = max(0U, min(r, 255U));
	G = max(0U, min(g, 255U));
	B = max(0U, min(b, 255U));
}

uint const RGB::getR() {
	return R;
}

uint const RGB::getG() {
	return G;
}

uint const RGB::getB() {
	return B;
}

RGB RGB::operator*(double multiplier) {
	return RGB(
		this->getR() * multiplier,
		this->getG() * multiplier,
		this->getB() * multiplier
	);
}

const string RGB::toString() {
	return "(R = " + to_string(R) + ";G = " + to_string(G) + ";B = " + to_string(B) + ")";
}

const string RGB::toHEX() {
	stringstream stream;
	stream << static_cast<unsigned char>(this->getR());
	stream << static_cast<unsigned char>(this->getG());
	stream << static_cast<unsigned char>(this->getB());
	return stream.str();
}