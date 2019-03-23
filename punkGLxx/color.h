#pragma once
#include <string>

using namespace std;
typedef unsigned int uint;

class RGB {
public:
	RGB();
	RGB(uint r, uint g, uint b);
	uint const getR();
	uint const getG();
	uint const getB();
	RGB operator*(double multiplier);
	string const toString();
	string const toHEX();
private:
	uint R;
	uint G;
	uint B;
};