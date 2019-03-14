// punkGLxx.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

typedef unsigned int uint;

using namespace std;

class RGB_color {
public:
	uint R;
	uint G;
	uint B;

	string toString() {
		return "(R: " + to_string(R) + " G: " + to_string(G) + " B: " + to_string(B) + ")";
	}
};

class Canvas {
	public:
		Canvas(uint x, uint y, RGB_color bg_color); // Конструктор
		void putPoint(uint x, uint y, RGB_color color); // Красим точку
		void dump();
	private:
		vector<vector<RGB_color>> holder;
		uint x_dim;
		uint y_dim;
};

Canvas::Canvas(uint x, uint y, RGB_color bg_color) {
	x_dim = x;
	y_dim = y;
	holder.resize(x, std::vector<RGB_color>(y, bg_color));
}

void Canvas::putPoint(uint x, uint y, RGB_color color) {
	// Кидаем если точка за границами холста
	if (x > x_dim || y > y_dim) throw overflow_error("Index bigger than canvas bounds");
	holder[x][y] = color;
}

void Canvas::dump() {
	for (uint x = 0; x < x_dim; x++) {
		for (uint y = 0; y < y_dim; y++) {
			cout << holder[x][y].toString() << " ";
		}
		cout << endl;
	}
}

int main()
{
	Canvas canvas(3, 4, RGB_color {125, 125, 125});
	canvas.putPoint(1, 1, RGB_color{ 255, 255, 255 } );
	canvas.putPoint(2, 3, RGB_color{ 255, 255, 255 });
	canvas.dump();
}