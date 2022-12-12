#pragma once
#include "Point.h"
class Ray
{
public:
	Ray(Point origin, Point direction) : 
		origin(origin),
		direction(direction) 
	{};
	Point origin;
	Point direction;
};

