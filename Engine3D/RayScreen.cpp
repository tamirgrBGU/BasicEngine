#include "RayScreen.h"
#include "RayCamera.h"
#include "Ray.h"

void RayScreen::SetPixelDimensions(int widthPixels, int heightPixels)
{
	this->widthPixels = widthPixels;
	this->heightPixels = heightPixels;
	xDirectionOnscreen = upperRightPoint.Sub(upperLeftPoint).Normalised();
	yDirectionOnscreen = lowerLeftPoint.Sub(upperLeftPoint).Normalised();

}

Ray RayScreen::ConstructRayThroughPixel(RayCamera camera, int row, int col)
{
	Point origin = PixelsToPoint(row, col);
	Point direction = origin.Sub(camera.location).Normalised();
	return Ray(origin, direction);
}

Point RayScreen::PixelsToPoint(int row, int col)
{
	double pixelWidthInSU = upperLeftPoint.Distance(upperRightPoint) / widthPixels;
	double pixelHeightInSU = upperLeftPoint.Distance(lowerLeftPoint) / heightPixels;
	Point x = xDirectionOnscreen.Scale((col+0.5)*pixelWidthInSU).Add(upperLeftPoint);
	Point y = yDirectionOnscreen.Scale((row+0.5)*pixelHeightInSU).Add(upperLeftPoint);
	return x.Add(y).Sub(upperLeftPoint);
}

