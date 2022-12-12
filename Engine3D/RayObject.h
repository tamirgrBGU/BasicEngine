#pragma once
#include "Ray.h"
#include "Intersection.h"
#include "Point.h"
class RayObject
{
public:
	virtual Intersection* Intersect(Ray in_ray) =0;
};

class Sphere : public RayObject
{
public:
	Sphere(Point origin, double radius) : origin(origin), radius(radius) {  };
	virtual Intersection* Intersect(Ray in_ray) ;
private:
	Point origin;
	double radius;
};

class Plane : public RayObject
{
	Plane(double a, double b, double c, double d) :a(a), b(b), c(c), d(d) {};
	virtual Intersection* Intersect(Ray in_ray) ;
private:
	double a;
	double b;
	double c;
	double d;
};
