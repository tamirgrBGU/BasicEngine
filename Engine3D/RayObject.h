#pragma once
#include "Ray.h"
#include "Intersection.h"
#include "Point.h"
#include "Colour.h"
#include <vector>
class RayObject
{
public:
	RayObject() :colour(Colour(0, 0, 0, 0)), shininess(0), kReflection(0), kTransparency(0) {};
	virtual Intersection* Intersect(Ray in_ray) =0;
	virtual Colour ColourAtPoint(Point point) = 0;
	void SetColour(Colour colour) {
		this->colour = colour;
	}
	void MarkMirror() {
		kReflection = 1;
	}
	void MarkTransparent() {
		kTransparency = 1;
	}
	virtual std::vector<double> DiffuseAtPoint(Point point) = 0;
	
	virtual Point NormalAtPoint(Point point) = 0;
	double shininess;
	double kReflection;
	double kTransparency;

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
	virtual std::vector<double>  DiffuseAtPoint(Point point) ;
private:
	Point origin;
	double radius;
};

class RayPlane : public RayObject
{
public:
	RayPlane(Point normal, double d) : normal(normal.Normalised()), d(d) {};
	virtual Intersection* Intersect(Ray in_ray) ;
	virtual Colour ColourAtPoint(Point point);
	virtual Point NormalAtPoint(Point point);
	virtual std::vector<double>  DiffuseAtPoint(Point point) ;
private:
	double d;
	Point normal;
};
