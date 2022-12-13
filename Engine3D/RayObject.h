#pragma once
#include "Ray.h"
#include "Intersection.h"
#include "Point.h"
#include "Colour.h"
class RayObject
{
public:
	RayObject() :colour(Colour(0, 0, 0, 0)), shininess(0) {};
	virtual Intersection* Intersect(Ray in_ray) =0;
	virtual Colour ColourAtPoint(Point point) = 0;
	void SetColour(Colour colour) {
		this->colour = colour;
	}
	virtual Point NormalAtPoint(Point point) = 0;
	double shininess;
protected:
	Colour colour;
};

class Sphere : public RayObject
{
public:
	Sphere(Point origin, double radius) : origin(origin), radius(radius), RayObject() {  };
	virtual Intersection* Intersect(Ray in_ray) ;
	virtual Colour ColourAtPoint(Point point);
	virtual Point NormalAtPoint(Point point);
private:
	Point origin;
	double radius;
};

class RayPlane : public RayObject
{
public:
	RayPlane(Point normal, double d) : normal(normal), d(d) {};
	virtual Intersection* Intersect(Ray in_ray) ;
	virtual Colour ColourAtPoint(Point point);
	virtual Point NormalAtPoint(Point point);
private:
	double d;
	Point normal;
};
