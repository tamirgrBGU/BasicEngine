#pragma once
#include "Colour.h"
#include "Intersection.h"
class RayLightsource
{
public:
	virtual Colour LightIntersection(Intersection* hit) = 0;
	RayLightsource(double rScale, double gScale, double bScale, double iScale) :
		rScale(rScale), gScale(gScale), bScale(bScale), iScale(iScale) {};
	void SetIntensity(double rScale, double gScale, double bScale, double iScale) {
		this->rScale = rScale;
		this->gScale = gScale;
		this->bScale = bScale;
		this->iScale = iScale;
	}
protected:
	double rScale;
	double gScale;
	double bScale;
	double iScale;

};

class Ambient : public RayLightsource
{
public:
	Ambient(double rScale, double gScale, double bScale, double iScale) : 
		RayLightsource(rScale, gScale, bScale, iScale){};
	virtual Colour LightIntersection(Intersection* hit);
};

class Directional : public RayLightsource
{
public:
	Directional(Point direction) : direction(direction), RayLightsource(0,0,0,0) {};
	virtual Colour LightIntersection(Intersection* hit);
private:
	Point direction;
};

class Spotlight : public RayLightsource
{
	virtual Colour LightIntersection(Intersection* hit);
};

