#include "pch.h"
#include "canvas.h"
#include <iomanip>
#include "render.h"

typedef unsigned int uint;

using namespace std;


int main()
{
	RenderParams params;
	Scene scene;
	Canvas light = render(params, scene, true);
	light.ssaa(4, 32, 0.005, 1, params, scene);
	light.dumpBMP("news_struct.bmp");
}