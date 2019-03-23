#include "pch.h"
#include "render.h"
#include <iostream>

Vector3D canvasToViewport(int x, int y, uint viewport_size, double projection_plane_z, Canvas &canvas) {
	return {
		x * double(viewport_size) / canvas.getWidth(),
		y * double(viewport_size) / canvas.getHeight(),
		projection_plane_z
	};
}

Vector3D canvasToViewportOffset(int x, int y, double offx, double offy, uint viewport_size, uint projection_plane_z, Canvas &canvas) {

	return Vector3D(
		(x + offx) * double(viewport_size) / canvas.getWidth(),
		(y + offy) * double(viewport_size) / canvas.getHeight(),
		projection_plane_z
	);
}

double calculateLightning(Vector3D &point, Vector3D &normal, Vector3D &view, Scene::Lights &lights, double specular) {
	double intensity = 0.0;

	// Ambient
	intensity += lights.ambient_light.getIntensity();
	// Point & Directional
	for (auto &point_l : lights.point_lights) {
		Vector3D light_vector = (point_l.getPosition() - point);
		intensity += point_l.calculateReflections(normal, view, light_vector, specular);
	}
	for (auto &direct_l : lights.directional_lights) {
		Vector3D light_pos_pointer = (direct_l.getPosition()); // Сэйв памяти по максимуму. В функцию не пихается, т.к. замкнутый указатель.
		intensity += direct_l.calculateReflections(normal, view, light_pos_pointer, specular);
	}
	return intensity;
}

RGB traceRay(Vector3D &origin, Vector3D &direction, double min_t, double max_t, Scene &scene, RGB &backgroundColor, bool withLight) {
	double closest_t = numeric_limits<double>::infinity();
	Sphere* closest_sphere = nullptr;

	for (auto &sphere : scene.objects.spheres) {
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
		double lightning = calculateLightning(point, normal, view, scene.lights, closest_sphere->getSpecular());
		RGB sphere_color = closest_sphere->getColor(); // const_cast zaebal :(
		return sphere_color * lightning;
	}
	return closest_sphere->getColor();
}

Canvas render(RenderParams &params, Scene &scene, bool withLight) {
	RGB bgc = params.backgroundColor;
	Canvas canvas(params.IMAGE_WIDTH, params.IMAGE_HEIGHT, bgc);
	cout << "[I] Starting render on " << params.IMAGE_WIDTH << "x" << params.IMAGE_HEIGHT << "px canvas..." << endl;
	long pixel_count = params.IMAGE_WIDTH * params.IMAGE_HEIGHT;
	const int w = params.IMAGE_WIDTH; const int h = params.IMAGE_HEIGHT;
	for (int x = -w / 2; x < w / 2; x++) {
		for (int y = -h / 2; y < h / 2; y++) {
			if (x == 0 && y == 0) cout << "[I] 50% Done..." << endl;
			Vector3D direction = canvasToViewport(x, y, params.viewport_size, params.projection_plane_z, canvas);
			Vector3D cameraPos = params.cameraPosition; // Придумать как задоджить такое разыменование const
			RGB bgcolor = params.backgroundColor;
			RGB pointColor = traceRay(cameraPos, direction, 1, numeric_limits<double>::infinity(), scene, bgcolor, withLight);
			canvas.put_point(x, y, pointColor);
		}
	}
	cout << "[I] Done!" << endl;
	return canvas;
}