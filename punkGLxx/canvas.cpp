#include "pch.h"
#include <iostream>
#include <fstream>
#include "color.h"
#include "canvas.h"
#include <ctime>
#include "nd_vector.h"
#include "render.h"

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


//AAsing
bool Canvas::sample_needed(int x, int y, double threshold) {
	struct CHolder {
		double r, g, b;
	};
	CHolder brightest{ -1,-1,-1 }, darkest{ 256,256,256 }, current;

	for (int lx = -1; lx < 2; lx++) {
		for (int ly = -1; ly < 2; ly++) {
			if (x + lx >= 0 && x + lx < this->getWidth() && y + ly >= 0 && y + ly < this->getHeight()) {
				current.r = holder[x + lx][y + ly].getR();
				current.g = holder[x + lx][y + ly].getG();
				current.b = holder[x + lx][y + ly].getB();

				if (current.r > brightest.r) {
					brightest.r = current.r;
				}
				if (current.g > brightest.g) {
					brightest.g = current.g;
				}
				if (current.b > brightest.b) {
					brightest.b = current.b;
				}

				if (current.r < darkest.r) {
					darkest.r = current.r;
				}
				if (current.g < darkest.g) {
					darkest.g = current.g;
				}
				if (current.b < darkest.b) {
					darkest.b = current.b;
				}
			}
		}
	}



	current.r = holder[x][y].getR();
	current.g = holder[x][y].getG();
	current.b = holder[x][y].getB();

	if (darkest.r == brightest.r && darkest.g == brightest.g && darkest.b == brightest.b && brightest.r == current.r && brightest.g == current.g && darkest.b == current.b) {
		return 0;
	}
	//<охуенный код>
	return (fabs(brightest.r / 255 - current.r / 255) > threshold || fabs(brightest.g / 255 - current.g / 255) > threshold || fabs(brightest.b / 255 - current.b / 255) > threshold || fabs(current.r / 255 - darkest.r / 255) > threshold || fabs(current.g / 255 - darkest.g / 255) > threshold || fabs(current.b / 255 - darkest.b / 255) > threshold);
	//</охуенный код>
}


void Canvas::ssaa(uint samples, uint rays, double threshold, double offset, RenderParams &params, Scene &scene) { //количество сэмплов, количество лучей на пиксель, гладкость(чем меньше, тем чувствительнее), разброс лучей
	const int w = this->getWidth(); const int h = this->getHeight();
	const bool withLight = true;
	int rays_shot{ 0 }, pixels_sampled{ 0 };
	srand(static_cast<unsigned int>(time(nullptr)));

	struct Rgb {
		RGB color;
	};

	RGB sas;

	for (int i = 0; i < samples; i++) {
		int xr{ -1 };
		for (int x = -w / 2; x < w / 2; x++) {
			int yr{ -1 };
			xr++;
			for (int y = -h / 2; y < h / 2; y++) {
				yr++;
				if (this->sample_needed(yr, xr, threshold)) {
					pixels_sampled++;//счетчик для дебага
					//<卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐__С++ИЛА РУ$И__卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍>

					cout << -x - 1 << "+" << y << " ";
					vector<Rgb> arr;
					arr.resize(rays);
					for (int j = 0; j < rays; j++) {
						rays_shot++;
						double offset_x, offset_y;

						offset_x = offset * rand() / RAND_MAX - offset / 2;
						offset_y = offset * rand() / RAND_MAX - offset / 2;

						Vector3D direction = canvasToViewportOffset(y, -x - 1, offset_x, offset_y, params.viewport_size, params.projection_plane_z, *this);
						Vector3D cameraPos = params.cameraPosition;
						RGB bgcolor = params.backgroundColor;
						arr[j].color = traceRay(cameraPos, direction, 1, numeric_limits<double>::infinity(), scene, bgcolor, withLight);
						sas = arr[j].color;
					}

					double r{ 0 }, g{ 0 }, b{ 0 };
					for (int i = 0; i < arr.size(); i++) {
						r += arr[i].color.getR();
						g += arr[i].color.getG();
						b += arr[i].color.getB();
					}
					RGB average{ static_cast<uint>(round(r / arr.size())), static_cast<uint>(round(g / arr.size())), static_cast<uint>(lround(b / arr.size())) };
					RGB marker{ 128,128,128 };
					this->put_point(y, -x - 1, average);
					//</卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐卐СИЛА РУ$И卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍卍>
				}
			}
		}
	}
	cout << pixels_sampled << " " << rays_shot;
}
