#pragma once
#include "Point.h"
#include "RayCamera.h"
#include "Ray.h"
class RayScreen
{
public:
	RayScreen(Point upperLeftPoint, Point upperRightPoint, Point lowerLeftPoint) :
		upperLeftPoint(upperLeftPoint),
		upperRightPoint(upperRightPoint),
		lowerLeftPoint(lowerLeftPoint),
		widthPixels(800),
		heightPixels(800),
		xDirectionOnscreen(Point(0,0,0)),
		yDirectionOnscreen(Point(0,0,0))
	{

	}

	void SetPixelDimensions(int widthPixels, int heightPixels);
	Ray ConstructRayThroughPixel(RayCamera camera, int row, int col);
	Point PixelsToPoint(int row, int col);
private:
	Point upperLeftPoint;
	Point upperRightPoint;
	Point lowerLeftPoint;
	int widthPixels;
	int heightPixels;
	Point xDirectionOnscreen;
	Point yDirectionOnscreen;
};

