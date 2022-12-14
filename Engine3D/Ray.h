#pragma once
#include "Point.h"
#include "Intersection.h"
class Ray
{
public:
	Ray(Point origin, Point direction) : 
		origin(origin),
		direction(direction),
		refractiveIndex(1)
	{};
	Point origin;
	Point direction;
	double refractiveIndex;
	Ray ReflectOff(Intersection* hit);
	Ray RefractAt(Intersection* hit);
};

