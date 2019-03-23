#pragma once
#include "primitives.h"
#include "light.h"

struct RenderParams {
	const uint IMAGE_WIDTH = 64U;
	const uint IMAGE_HEIGHT = 64U;
	const uint viewport_size = 1U;
	const uint projection_plane_z = 1U;
	const Vector3D cameraPosition = Vector3D(0, 0, 0);
	const RGB backgroundColor = RGB(255, 255, 255);
};

struct Scene {

	// Scene objects
	struct Objects {
		vector<Sphere> spheres;
	};

	// Lighting
	struct Lights {
		Light ambient_light; // Эмбиент всегда один на сцене
		vector<Light> point_lights;
		vector<Light> directional_lights;
	};

	Objects objects = { {
			Sphere(Vector3D(0, -1, 3), 1, RGB(255, 50, 100), 250),
			Sphere(Vector3D(2, 0, 4), 1, RGB(130, 250, 50), 500),
			Sphere(Vector3D(-2, 0, 4), 1, RGB(100, 200, 255), 10),
			Sphere(Vector3D(-2, 2, 4), 1.5, RGB(128, 128, 128), 1000) // Цвет Петербурга
		} };
	Lights lights = { Light(0.2, Vector3D(0.0, 0.0, 0.0)),
		{ Light(0.6, Vector3D(2.0, 1.0, 0.0)) },
		{ Light(0.2, Vector3D(1.0, 4.0, 4.0)) }
	};
};
