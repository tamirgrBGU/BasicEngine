#pragma once
#include "Colour.h"
#include "Intersection.h"
#include "Ray.h"
#include "math.h"
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
	virtual void SetCutoffCosAlpha(double cosAlpha)=0;
	virtual double DistanceToPoint(Point point) = 0;
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
	virtual void SetCutoffCosAlpha(double cosAlpha) {
		return;
	}
	virtual double DistanceToPoint(Point point) { return INFINITY; };
};

class Directional : public RayLightsource
{
public:
	Directional(Point direction) : direction(direction.Normalised()), RayLightsource(0,0,0,0) {};
	virtual Colour LightIntersection(Intersection* hit);
	virtual Ray* ConstructLightRay(Point point);
	virtual void SetOrigin(Point origin) {
		return;
	}
	virtual void SetCutoffCosAlpha(double cosAlpha) {
		return;
	}
	virtual double DistanceToPoint(Point point) { return INFINITY; };
private:
	Point direction;
};

class Spotlight : public RayLightsource
{
public:
	Spotlight(Point direction) : direction(direction.Normalised()), RayLightsource(0, 0, 0, 0), origin(Point(0, 0, 0)), cutoffCosAlpha(0) {};
	virtual Colour LightIntersection(Intersection* hit);
	virtual Ray* ConstructLightRay(Point point);
	virtual void SetOrigin(Point origin) {
		this->origin = origin;
	}
	virtual void SetCutoffCosAlpha(double cosAlpha);
	virtual double DistanceToPoint(Point point);

private:
	Point origin;
	Point direction;
	double cutoffCosAlpha;
};

