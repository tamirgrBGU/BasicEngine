#pragma once
#include "Point.h"
#include "RayCamera.h"
#include "Ray.h"
#include <vector>
#include <vector>
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
	std::vector<Ray> ConstructRaysThroughPixel(RayCamera camera, int row, int col, int samples);
	std::vector<Point> PixelsToPoints(int row, int col, int samples);
private:
	Point upperLeftPoint;
	Point upperRightPoint;
	Point lowerLeftPoint;
	int widthPixels;
	int heightPixels;
	Point xDirectionOnscreen;
	Point yDirectionOnscreen;
};

