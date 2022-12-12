#pragma once
#include "Point.h"
class RayObject;
class Intersection
{
public:
	Point point;
	RayObject* object;
	Intersection(Point point, RayObject* object): point(point), object(object) {};
};

