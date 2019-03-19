#include "pch.h"
#include "nd_vector.h"
#include "color.h"
#include "canvas.h"
#include "light.h"
#include "primitives.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <string>

typedef unsigned int uint;

using namespace std;

// Renderer
// TODO поместить это зло в нужное место.
// Настройки теперь тут, чтобы писать в хэдэр
const uint IMAGE_WIDTH = 512;
const uint IMAGE_HEIGHT = 512;
const uint viewport_size = 1;
const uint projection_plane_z = 1;
const Vector3D cameraPosition(0, 0, 0);
const RGB backgroundColor(255, 255, 255);
vector<Sphere> spheres{
	Sphere(Vector3D(0, -1, 3), 1, RGB(255, 50, 100), 250),
	Sphere(Vector3D(2, 0, 4), 1, RGB(130, 250, 50), 500),
	Sphere(Vector3D(-2, 0, 4), 1, RGB(100, 200, 255), 10),
	Sphere(Vector3D(-2, 2, 4), 1.5, RGB(128, 128, 128), 1000) // Цвет Петербурга
};
Light ambient_light(0.2, Vector3D(0.0, 0.0, 0.0)); // Эмбиент всегда один на сцене
vector<Light> point_lights{
	Light(0.6, Vector3D(2.0, 1.0, 0.0))
};
vector<Light> directional_lights{
	Light(0.2, Vector3D(1.0, 4.0, 4.0))
};

const Vector3D canvasToViewport(int x, int y, Canvas &canvas) {
	return Vector3D(
		x * double(viewport_size) / canvas.getWidth(),
		y * double(viewport_size) / canvas.getHeight(),
		projection_plane_z
	);
}

double calculateLightning(Vector3D &point, Vector3D &normal, Vector3D &view, double specular) {
	double intensity = 0.0;

	// Ambient
	intensity += ambient_light.getIntensity();
	// Point & Directional
	for (auto &point_l : point_lights) {
		Vector3D light_vector = (point_l.getPosition() - point);
		intensity += point_l.calculateReflections(normal, view, light_vector, specular);
	}
	for (auto &direct_l : directional_lights) {
		Vector3D light_pos_pointer = (direct_l.getPosition()); // Сэйв памяти по максимуму. В функцию не пихается, т.к. замкнутый указатель.
		intensity += direct_l.calculateReflections(normal, view, light_pos_pointer, specular);
	}
	return intensity;
}

RGB traceRay(Vector3D &origin, Vector3D &direction, double min_t, double max_t, bool withLight) {
	double closest_t = numeric_limits<double>::infinity();
	Sphere* closest_sphere = nullptr;

	for (auto &sphere : spheres) {
		Vector2D ts = sphere.intersectWithRay(origin, direction);
		if (ts.getX() < closest_t && min_t < ts.getX() && ts.getX() < max_t) {
			closest_t = ts.getX();
			closest_sphere = &sphere;
		}
		if (ts.getY() < closest_t && min_t < ts.getY() && ts.getY() < max_t) {
			closest_t = ts.getY();
			closest_sphere = &sphere;
		}
	}

	if (closest_sphere == nullptr)
		return backgroundColor;

	if (withLight) {
		Vector3D point = (direction * closest_t) + origin;
		Vector3D closest_sphere_center = closest_sphere->getCenter(); // Сука const_cast так и не заработал ни разу.
		Vector3D normal_t = point - closest_sphere_center; // Тоже простор для оптимизации по памяти
		Vector3D normal = normal_t * (1.0 / normal_t.length());

		Vector3D view = direction * -1.0;
		double lightning = calculateLightning(point, normal, view, closest_sphere->getSpecular());
		RGB sphere_color = closest_sphere->getColor(); // const_cast zaebal :(
		return sphere_color * lightning;
	}
	else
		return closest_sphere->getColor();
}

Canvas render(bool withLight) {
	RGB bgc = backgroundColor;
	Canvas canvas(IMAGE_WIDTH, IMAGE_HEIGHT, bgc);
	cout << "[I] Starting render on " << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << "px canvas..." << endl;
	long pixel_count = IMAGE_WIDTH * IMAGE_HEIGHT;
	const int w = IMAGE_WIDTH; const int h = IMAGE_HEIGHT;
	for (int x = -w / 2; x < w / 2; x++) {
		for (int y = -h / 2; y < h / 2; y++) {
			if (x == 0 && y == 0) cout << "[I] 50% Done..." << endl;
			Vector3D direction = canvasToViewport(x, y, canvas);
			Vector3D cameraPos = cameraPosition; // Придумать как задоджить такое разыменование const 
			RGB pointColor = traceRay(cameraPos, direction, 1, numeric_limits<double>::infinity(), withLight);
			canvas.put_point(x, y, pointColor);
		}
	}
	cout << "[I] Done!" << endl;
	return canvas;
}

int main()
{
	Canvas light = render(true);
	light.dumpBMP("newstruct.bmp");
}