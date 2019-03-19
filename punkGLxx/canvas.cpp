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
	// ������ ���� ����� �� ��������� ������
	// ����� ��������� �� 0
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

// �������� ���������� ����� �� ����, ������� ����� �������� � ���� ��� ���� � BMP. 
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
	// ����� ���� � ���� � ������ ������
	outfile << this->getWidth() << ";" << this->getHeight() << endl;
	// ����� ��� ������� � ���� ������, �� ��������� ��� ��� �� 6 ������
	for (uint x = 0; x < this->getWidth(); x++)
		for (uint y = 0; y < this->getHeight(); y++)
			outfile << holder[x][y].toHEX();
	outfile.close();
}

//������ �� �������� �������� image_width � image_height ��� ������ � ����

//���������������� ����� � ����� ���������, ����� �� ������ ��� ��� ������ ��������
void Canvas::dumpBMP(string filename) {

	struct Header
	{

		//file header
		//char b - ���������� ������ ����� �������� �����
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

	int padding; // ������� ������, �� ���� ���������� ������, ������� ��� ���� ���������, ����� �������� ������ ������� 4
	padding = 4 - (3 * this->getWidth()) % 4;
	if (padding == 4)
	{
		padding = 0; // � ���� � ��� �����
	}

	Header x;
	//x.b = 1; ����������� �� ���� ���� ����� �� �������, ������ ������� �� ����, ��� ����� � ������
	x.size = sizeof(x) + this->getWidth() * this->getHeight() * 3 + padding * this->getHeight() + 2; //+2 ����� �� 2 ������������ �����
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
	//��������� ���� �� ����������
	ofstream output_file(filename, ofstream::binary);
	output_file.write("BM", 2); // � ��� � ������������� 2 ����� ������
	output_file.write((char*)&x, sizeof(x)); // ����������� ��������� � �������� �������

	//����������� ������		
	char dummy{ 0 }; // ���������� ��� �������� � �������� ��������

	//ofstream output_file(filename, ios::app); //������� �� ����������

	//������
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
					output_file << dummy; //����� ���� ������� �� ������� "������" ��������� ����� � ����������, ������� ��������� �������
				}
			}
		}
	}
	output_file.close();
	cout << "[I] Done!" << endl;
}