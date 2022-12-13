#include "RayScene.h"
#include "Image.h"
#include "Ray.h"


unsigned char* RayScene::Render(int widthPixels, int heightPixels, int samples)
{
	Image image = Image(widthPixels, heightPixels);
	screen.SetPixelDimensions(widthPixels, heightPixels);
	for (int row = 0; row < heightPixels; row++) {
		for (int col = 0; col < widthPixels; col++) {
			std::vector<Ray> rays = screen.ConstructRaysThroughPixel(camera, row, col, samples);
			std::vector<Colour> colours = std::vector<Colour>();
			for (auto ray : rays) {
				colours.push_back(CalcColour(ray));
			}
			int totalRed = 0;
			int totalGreen = 0;
			int totalBlue = 0;
			int totalIntensity = 0;
			for (auto colour : colours) {
				totalRed += colour.red;
				totalBlue += colour.blue;
				totalGreen += colour.green;
				totalIntensity += colour.intensity;
			}
			Colour avg = Colour((unsigned char)(totalRed / colours.size()),
				(unsigned char)(totalGreen / colours.size()),
				(unsigned char)(totalBlue / colours.size()),
				(unsigned char)(totalIntensity / colours.size()));
			image.data.push_back(avg);
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

void RayScene::ColourRayObject(int index, Colour colour)
{
	rayObjects[index]->SetColour(colour);
}

void RayScene::SetRayObjectShininess(int index, double shininess)
{
	rayObjects[index]->shininess = shininess;
}

void RayScene::AddRayLightsource(RayLightsource* rayLightsource)
{
	lightsources.push_back(rayLightsource);
}

void RayScene::SetLightsourceIntensity(int index, double rScale, double gScale, double bScale, double iScale)
{
	lightsources[index]->SetIntensity(rScale, gScale, bScale, iScale);
}

void RayScene::SetLightsourceOrigin(int index, Point origin)
{
	lightsources[index]->SetOrigin(origin);
}

Colour RayScene::CalcColour(Ray in_ray)
{
	Intersection* hit = FindIntersection(in_ray);
	if (hit == nullptr) {
		return Colour(205,205,0,255);
	}
	Colour colour = Colour(0, 0, 0, 0);
	for (auto lightsource : lightsources) {
		Ray* lightRay = lightsource->ConstructLightRay(hit->point);
		if (lightRay == nullptr || !Occluded(*lightRay)) {
			colour = colour.Add(lightsource->LightIntersection(hit));
		}
	}
	return colour;

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

bool RayScene::Occluded(Ray lightRay)
{
	return FindIntersection(lightRay)==nullptr;
}
