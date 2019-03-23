#pragma once
#include "nd_vector.h"
#include "color.h"
#include "canvas.h"
#include "scene.h"

typedef unsigned int uint;
// Render tools

Canvas render(RenderParams& params, Scene& scene, bool withLight);
Vector3D canvasToViewport(int x, int y, uint viewport_size, double projection_plane_z, Canvas &canvas);
Vector3D canvasToViewportOffset(int x, int y, double offx, double offy, uint viewport_size, uint projection_plane_z, Canvas &canvas);
double calculateLightning(Vector3D &point, Vector3D &normal, Vector3D &view, Scene::Lights &lights, double specular);
RGB traceRay(Vector3D &origin, Vector3D &direction, double min_t, double max_t, Scene &objects, RGB &backgroundColor, bool withLight);

