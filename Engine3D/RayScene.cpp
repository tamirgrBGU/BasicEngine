#include "RayScene.h"
#include "Image.h"
#include "Ray.h"


unsigned char* RayScene::Render(int widthPixels, int heightPixels)
{
	Image image = Image(widthPixels, heightPixels);
	screen.SetPixelDimensions(widthPixels, heightPixels);
	for (int row = 0; row < heightPixels; row++) {
		for (int col = 0; col < widthPixels; col++) {
			Ray ray = screen.ConstructRayThroughPixel(camera, row, col);
			image.data.push_back(CalcColour(ray));
		}
	}
	return image.ToCharArray();
}

void RayScene::SetCamera(Point location)
{
	camera = RayCamera(location);
}

void RayScene::AddRayObject(RayObject* rayObject)
{
	rayObjects.push_back(rayObject);
}

void RayScene::AddRayLightsource(RayLightsource* rayLightsource)
{
	lightsources.push_back(rayLightsource);
}

Colour RayScene::CalcColour(Ray in_ray)
{
	Intersection* hit = FindIntersection(in_ray);
	if (hit == nullptr) {
		return Colour(0, 0, 255, 255);
	}
	return Colour(255,0,0,255);
}

Intersection* RayScene::FindIntersection(Ray in_ray)
{
	double min_t = INFINITY;
	Intersection* closest_intersection = nullptr;
	for (RayObject* object : rayObjects) {
		Intersection* intersection = object->Intersect(in_ray);
		if (intersection != nullptr) {
			double t = intersection->point.Distance(in_ray.origin);
			if (t < min_t) {
				closest_intersection = intersection;
				min_t = t;
			}
		}
	}
	return closest_intersection;
}
