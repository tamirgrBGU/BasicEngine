#pragma once
#include "Colour.h"
#include "Intersection.h"
#include "Ray.h"
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
	virtual Ray* ConstructLightRay(Point point) = 0;
	virtual void SetOrigin(Point origin)=0;
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
	virtual Ray* ConstructLightRay(Point point);
	virtual Colour LightIntersection(Intersection* hit);
	virtual void SetOrigin(Point origin) {
		return;
	}
};

class Directional : public RayLightsource
{
public:
	Directional(Point direction) : direction(direction), RayLightsource(0,0,0,0) {};
	virtual Colour LightIntersection(Intersection* hit);
	virtual Ray* ConstructLightRay(Point point);
	virtual void SetOrigin(Point origin) {
		return;
	}
private:
	Point direction;
};

class Spotlight : public RayLightsource
{
	Spotlight(Point direction) : direction(direction), RayLightsource(0, 0, 0, 0), origin(Point(0, 0, 0)) {};
	virtual Colour LightIntersection(Intersection* hit);
	virtual Ray* ConstructLightRay(Point point);
	virtual void SetOrigin(Point origin) {
		this->origin = origin;
	}

private:
	Point origin;
	Point direction;
};

