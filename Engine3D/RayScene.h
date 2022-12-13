#pragma once
#include <vector>
#include "RayObject.h"
#include "RayLightsource.h"
#include "RayScreen.h"
#include "RayCamera.h"
#include "Intersection.h"
#include "Colour.h"
using namespace std;
class RayScene
{
public:
	RayScene() : 
		screen(RayScreen(Point(-1, 1, 0), Point(1, 1, 0), Point(-1, -1, 0))), 
		rayObjects(vector<RayObject*>()), 
		lightsources(vector<RayLightsource*>()) 
	{};

	unsigned char* Render(int widthPixels, int heightPixels, int samples);
	void SetCamera(Point location);
	void AddRayObject(RayObject* rayObject);
	void ColourRayObject(int index, Colour colour);
	void SetRayObjectShininess(int index, double shininess);
	void AddRayLightsource(RayLightsource* rayLightsource);
	void SetLightsourceIntensity(int index, double rScale, double gScale, double bScale, double iScale);
	Colour CalcColour(Ray in_ray);
	Intersection* FindIntersection(Ray in_ray);
	

private:
	vector<RayObject*> rayObjects;
	vector<RayLightsource*> lightsources;
	RayScreen screen;
	RayCamera camera;
};
