#pragma once
class Point
{
public:
	double x, y, z;
	Point(double x, double y, double z) : x(x), y(y), z(z){}
	double Distance(Point point);
	double DistanceX(Point point);
	double DistanceY(Point point);
	double DistanceZ(Point point);
	Point Normalised();
	Point Scale(double scale);
	Point Add(Point point);
	Point Sub(Point point);
	double Dot(Point point);
	double Magnitude();
};

