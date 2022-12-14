#include "RayScene.h"
#include "Image.h"
#include "Ray.h"


unsigned char* RayScene::Render(int widthPixels, int heightPixels, int samples, int recursionLevels)
{
	Image image = Image(widthPixels, heightPixels);
	screen.SetPixelDimensions(widthPixels, heightPixels);
	for (int row = 0; row < heightPixels; row++) {
		for (int col = 0; col < widthPixels; col++) {
			std::vector<Ray> rays = screen.ConstructRaysThroughPixel(camera, row, col, samples);
			std::vector<Colour> colours = std::vector<Colour>();
			for (auto& ray : rays) {
				colours.push_back(CalcColour(ray, recursionLevels, 0));
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

void RayScene::SetLightsourceCutoffCosAlpha(int index, double cosAlpha)
{
	lightsources[index]->SetCutoffCosAlpha(cosAlpha);
}

Colour RayScene::CalcColour(Ray in_ray, int maxRecursionLevels, int currentRecursionLevel)
{
	Colour colour = Colour(0, 0, 0, 0);
	if (currentRecursionLevel == maxRecursionLevels) {
		return colour;
	}
	Intersection* hit = FindIntersection(in_ray);
	if (hit == nullptr) {
		return colour;
	}
	if (hit->object->kReflection != 1 && hit->object->kTransparency != 1) {
		for (auto lightsource : lightsources) {
			Ray* lightRay = lightsource->ConstructLightRay(hit->point);
			if (lightRay == nullptr || !Occluded(*lightRay, lightsource->DistanceToPoint(hit->point))) {
				colour = colour.Add(lightsource->LightIntersection(hit));
			}
			free(lightRay);
			/*else {
				if (hit->object->ColourAtPoint(Point(0,0,0)).red==255 && Occluded(*lightRay)) {
					Intersection* test = FindIntersection(*lightRay);
					continue;
				}
			}*/
		}
	}

		// Reflection recursion
	if (hit->object->kReflection != 0) {
		double kReflection = hit->object->kReflection;
		Ray out_ray = in_ray.ReflectOff(hit);
		colour = colour.Add(CalcColour(out_ray, maxRecursionLevels, currentRecursionLevel + 1)).
			Multiply(kReflection,kReflection,kReflection,kReflection);
	}

	// Transparent recursion
	if (hit->object->kTransparency != 0) {
		double kTransparency = hit->object->kTransparency;
		Ray out_ray = in_ray.RefractAt(hit);
		colour = colour.Add(CalcColour(out_ray, maxRecursionLevels, currentRecursionLevel + 1)).
			Multiply(kTransparency, kTransparency, kTransparency, kTransparency);
	}

	free(hit);
	return colour;
}

Intersection* RayScene::FindIntersection(Ray in_ray)
{
	double min_t = INFINITY;
	Intersection* intersection;
	Intersection* closest_intersection = nullptr;
	for (RayObject* object : rayObjects) {
		intersection = object->Intersect(in_ray);
		if (intersection != nullptr) {
			double t = intersection->point.Distance(in_ray.origin);
			if (t < min_t) {
				if (closest_intersection != nullptr) {
					free(closest_intersection);
				}
				closest_intersection = intersection;
				min_t = t;
			}
			else {
				free(intersection);
			}
		}
	}
	return closest_intersection;
}

bool RayScene::Occluded(Ray lightRay, double maxDistance)
{
	Intersection* hit = FindIntersection(Ray(lightRay.origin.Add(lightRay.direction.Scale(0.01)), lightRay.direction));
	bool result = hit!=nullptr && hit->point.Distance(lightRay.origin) < maxDistance;
	free(hit);
	return result;
}
