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

std::vector<Ray> RayScreen::ConstructRaysThroughPixel(RayCamera camera, int row, int col, int samples)
{
	std::vector<Ray> rays = std::vector<Ray>();
	std::vector<Point> origins = PixelsToPoints(row, col, samples);
	for (auto origin : origins) {
		Point direction = origin.Sub(camera.location).Normalised();
		rays.push_back(Ray(origin, direction));
	}
	return rays;
}

std::vector<Point> RayScreen::PixelsToPoints(int row, int col, int samples)
{
	double pixelWidthInSU = upperLeftPoint.Distance(upperRightPoint) / widthPixels;
	double pixelHeightInSU = upperLeftPoint.Distance(lowerLeftPoint) / heightPixels;

	// Curently only suppport 4x samples
	std::vector<Point> points = std::vector<Point>();
	if (samples == 4) {
		
		Point x = xDirectionOnscreen.Scale((col+0.25)*pixelWidthInSU).Add(upperLeftPoint);
		Point y = yDirectionOnscreen.Scale((row+0.25)*pixelHeightInSU).Add(upperLeftPoint);
		points.push_back(x.Add(y).Sub(upperLeftPoint));
		 x = xDirectionOnscreen.Scale((col+0.75)*pixelWidthInSU).Add(upperLeftPoint);
		 y = yDirectionOnscreen.Scale((row+0.25)*pixelHeightInSU).Add(upperLeftPoint);
		points.push_back(x.Add(y).Sub(upperLeftPoint));
		 x = xDirectionOnscreen.Scale((col+0.25)*pixelWidthInSU).Add(upperLeftPoint);
		 y = yDirectionOnscreen.Scale((row+0.75)*pixelHeightInSU).Add(upperLeftPoint);
		points.push_back(x.Add(y).Sub(upperLeftPoint));
		 x = xDirectionOnscreen.Scale((col+0.75)*pixelWidthInSU).Add(upperLeftPoint);
		 y = yDirectionOnscreen.Scale((row+0.75)*pixelHeightInSU).Add(upperLeftPoint);
		points.push_back(x.Add(y).Sub(upperLeftPoint));
	}
	else {
		Point x = xDirectionOnscreen.Scale((col+0.5)*pixelWidthInSU).Add(upperLeftPoint);
		Point y = yDirectionOnscreen.Scale((row+0.5)*pixelHeightInSU).Add(upperLeftPoint);
		points.push_back(x.Add(y).Sub(upperLeftPoint));
	}
	return points;
}

