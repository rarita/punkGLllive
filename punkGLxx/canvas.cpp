#include "pch.h"
#include <iostream>
#include <fstream>
#include "color.h"
#include "canvas.h"

using namespace std;
typedef unsigned int uint;

Canvas::Canvas(uint x, uint y, RGB &bg_color) : x_dim(x), y_dim(y) {
	holder.resize(x, std::vector<RGB>(y, bg_color));
}

void Canvas::put_point(int x, int y, RGB &color) {
	// Кидаем если точка за границами холста
	// Точки считаются от 0
	uint real_x = this->getWidth() / 2 + x;
	uint real_y = this->getHeight() / 2 - y - 1;
	if (real_x > x_dim - 1 || real_y > y_dim - 1) throw overflow_error("Index bigger than canvas bounds");
	holder[real_x][real_y] = color;
}

uint const Canvas::getWidth() {
	return x_dim;
}

uint const Canvas::getHeight() {
	return y_dim;
}

// Временно вываливаем писюн на стол, сделать потом редирект в файл или пайп в BMP. 
void Canvas::dump() {
	for (uint x = 0; x < x_dim; x++) {
		for (uint y = 0; y < y_dim; y++) {
			RGB color = holder[x][y];
			cout << color.toString() << " ";
		}
		cout << endl;
	}
}

void Canvas::dumpHEX(string filename) {
	ofstream outfile;
	outfile.open(filename);
	// Пишем ширь и высь в первой строке
	outfile << this->getWidth() << ";" << this->getHeight() << endl;
	// Пишем все пикселы в след строке, не разделяем ибо все по 6 знаков
	for (uint x = 0; x < this->getWidth(); x++)
		for (uint y = 0; y < this->getHeight(); y++)
			outfile << holder[x][y].toHEX();
	outfile.close();
}

//Теперь из настроек пуллится image_width и image_height для записи в файл

//многострадальная хуйня с кучей комментов, чтобы не забыть как это вообще работает
void Canvas::dumpBMP(string filename) {

	struct Header
	{

		//file header
		//char b - прикручено руками после открытия файла
		long size;
		short reserved_field_1;
		short reserved_field_2;
		long offset;

		//DIB header
		long numberDIB;
		long width;
		long height;
		short planenumber;
		short depth;
		long comp;
		long rawsize;
		long reshor;
		long resver;
		long palette;
		long important;
	};

	int padding; // считаем отступ, то есть количество байтов, которые нам надо прибавить, чтобы получить строку кратную 4
	padding = 4 - (3 * this->getWidth()) % 4;
	if (padding == 4)
	{
		padding = 0; // я курю и мне похуй
	}

	Header x;
	//x.b = 1; большинство из этих штук можно не трогать, размер подаётся на вход, вся хуйня в байтах
	x.size = sizeof(x) + this->getWidth() * this->getHeight() * 3 + padding * this->getHeight() + 2; //+2 байта за 2 прикрученные буквы
	x.reserved_field_1 = 0;
	x.reserved_field_2 = 0;
	x.offset = sizeof(x) + 2;

	x.numberDIB = 40;
	x.width = this->getWidth();
	x.height = this->getHeight();
	x.planenumber = 1;
	x.depth = 24;
	x.comp = 0;
	x.rawsize = this->getWidth() * this->getHeight() * 3 + padding * this->getHeight();
	x.reshor = 0x0b13;
	x.resver = 0x0b13;
	x.palette = 0;
	x.important = 0;

	cout << "[I] Writing canvas to file " << filename << "..." << endl;
	//открываем файл на добавление
	ofstream output_file(filename, ofstream::binary);
	output_file.write("BM", 2); // а вот и прикручивание 2 буквы хэдэра
	output_file.write((char*)&x, sizeof(x)); // прописываем структуру в бинарном формате

	//прописываем массив		
	char dummy{ 0 }; // записываем это значение в качестве паддинга

	//ofstream output_file(filename, ios::app); //открыли на добавление

	//дрочим
	for (int i = this->getHeight() - 1; i >= 0; --i)
	{
		for (int j = 0; j <= this->getWidth(); ++j)
		{
			if (j < this->getWidth())
			{

				output_file << static_cast<unsigned char>(holder[j][i].getG());
				output_file << static_cast<unsigned char>(holder[j][i].getR());
				output_file << static_cast<unsigned char>(holder[j][i].getB());
			}
			else
			{
				for (long long n = 0; n < padding; ++n)
				{
					output_file << dummy; //когда цикл выходит за пределы "холста" добавляем чаров в количестве, которое посчитали заранее
				}
			}
		}
	}
	output_file.close();
	cout << "[I] Done!" << endl;
}