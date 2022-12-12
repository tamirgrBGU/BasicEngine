#pragma once
#include "Point.h"
class RayCamera
{
public:
	RayCamera() : location(Point(0,0,0)){}
	RayCamera(Point location): location(location){}
	Point location;
};

