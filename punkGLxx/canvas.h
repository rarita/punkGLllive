#pragma once
#include <vector>
#include "color.h"
typedef unsigned int uint;

class Canvas {
public:
	Canvas(uint x, uint y, RGB &bg_color); // Конструктор
	void put_point(int x, int y, RGB &color); // Красим точку
	void dump(); // Сливаем весь холст в нужный поток
	void dumpHEX(string filename); // То же самое но в HEX для экономии места
	void dumpBMP(string filename);
	uint const getWidth();
	uint const getHeight();
private:
	vector<vector<RGB>> holder;
	const uint x_dim;
	const uint y_dim;
};
