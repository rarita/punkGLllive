#pragma once
#include <vector>
#include "scene.h"
#include "color.h"
typedef unsigned int uint;

class Canvas {
public:
	Canvas(uint x, uint y, RGB &bg_color); // �����������
	void put_point(int x, int y, RGB &color); // ������ �����
	void ssaa(uint samples, uint rays, double threshold, double offset, RenderParams &params, Scene &scene);
	bool sample_needed(int x, int y, double threshold);
	void dump(); // ������� ���� ����� � ������ �����
	void dumpHEX(string filename); // �� �� ����� �� � HEX ��� �������� �����
	void dumpBMP(string filename);
	uint const getWidth();
	uint const getHeight();
private:
	vector<vector<RGB>> holder;
	const uint x_dim;
	const uint y_dim;
};
