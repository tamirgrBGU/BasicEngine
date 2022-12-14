#include "RayObject.h"
#include "Intersection.h"
#include "Ray.h"
#include <math.h>
#include <Colour.h>
#include <iostream>
Intersection* Sphere::Intersect(Ray in_ray)
{
	Point L = origin.Sub(in_ray.origin);
	double t_m = L.Dot(in_ray.direction);
	double d_sqr = pow(L.Magnitude(), 2) - pow(t_m, 2);
	if (d_sqr > pow(radius, 2)) {
		return nullptr;
	}
	double t_h = sqrt(pow(radius, 2) - d_sqr);
	
	double t;
	if (origin.Distance(in_ray.origin) < radius+0.01) {
		// ray came from inside sphere - supports transparency
		t = t_m + t_h;
	}
	else {
		t = t_m - t_h;
	}
	if (t < 0) {
		return nullptr;
	}
	Point hit = in_ray.direction.Scale(t).Add(in_ray.origin);

	return new Intersection(hit, this);
}

Colour Sphere::ColourAtPoint(Point point)
{
	return colour;
}

Point Sphere::NormalAtPoint(Point point)
{
	return point.Sub(origin).Normalised();
}

std::vector<double>  Sphere::DiffuseAtPoint(Point point)
{
	std::vector<double> diffuse = std::vector<double>();
	diffuse.push_back(1);
	diffuse.push_back(1);
	diffuse.push_back(1);
	diffuse.push_back(1);
	return diffuse;
}

std::vector<double>  RayPlane::DiffuseAtPoint(Point point)
{
	std::vector<double> diffuse = std::vector<double>();
	if ((point.x < 0 && point.y < 0) || (point.x >= 0 && point.y >= 0)) {
		if ((abs(int(1.5 * point.x)) % 2) == (abs(int(1.5 * point.y)) % 2)) {
			diffuse.push_back(0.5);
			diffuse.push_back(0.5);
			diffuse.push_back(0.5);
			diffuse.push_back(1);
			return diffuse;
		}
	}
	else
	{
		if ((abs(int(1.5 * point.x)) % 2) != (abs(int(1.5 * point.y)) % 2)) {
			diffuse.push_back(0.5);
			diffuse.push_back(0.5);
			diffuse.push_back(0.5);
			diffuse.push_back(1);
			return diffuse;
		}
	}
	diffuse.push_back(1);
	diffuse.push_back(1);
	diffuse.push_back(1);
	diffuse.push_back(1);
	return diffuse;
}

Intersection* RayPlane::Intersect(Ray in_ray)
{
	double NV = normal.Dot(in_ray.direction);
	if (NV == 0) {
		// Parallel to plane
		return nullptr;
	}
	Point Q_0 = Point(in_ray.origin.x,
		in_ray.origin.y,
		(-d - normal.x * in_ray.origin.x - normal.y * in_ray.origin.y) / normal.z);
	double t = normal.Dot(Q_0.Sub(in_ray.origin).Scale(1 / NV));
	if (t <= 0) {
		// Not infront of ray
		return nullptr;
	}
	Point hit = in_ray.origin.Add(in_ray.direction.Scale(t));
	if (hit.Sub(Q_0).Dot(normal) >= 0.000001) {
		std::cout << hit.Sub(Q_0).Dot(normal) << std::endl;
	}
	return new Intersection(hit, this);
}

Colour RayPlane::ColourAtPoint(Point point)
{
	return colour;
}

Point RayPlane::NormalAtPoint(Point point)
{
	return normal.Scale(-1);
}
