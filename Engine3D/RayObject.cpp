#include "RayObject.h"
#include "Intersection.h"
#include "Ray.h"
#include <math.h>
#include <Colour.h>
Intersection* Sphere::Intersect(Ray in_ray)
{
	Point L = origin.Sub(in_ray.origin);
	double t_m = L.Dot(in_ray.direction);
	double d_sqr = pow(L.Magnitude(), 2) - pow(t_m, 2);
	if (d_sqr > pow(radius, 2)) {
		return nullptr;
	}
	double t_h = sqrt(pow(radius, 2) - d_sqr);
	double t = t_m - t_h;
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
	return new Intersection(hit, this);
}

Colour RayPlane::ColourAtPoint(Point point)
{
	Point base = Point(0, 0, -d / normal.z);
	Point xDirection = Point(1, 0, (-d - normal.x) / normal.z).Sub(base).Normalised();
	Point yDirection = Point(base.y * xDirection.z - base.z * xDirection.y,
		base.z * xDirection.x - base.x * xDirection.z,
		base.x * xDirection.y - base.y * xDirection.x).Normalised();
	double x = xDirection.Dot(point);
	double y = yDirection.Dot(point);

	if ((x < 0 && y<0 )|| (x>=0 && y>=0)) {
		if ((abs(int(1.5 * x)) % 2) == (abs(int(1.5 * y))% 2)) {
			return colour.Multiply(1.5, 1.5, 1.5, 1);
		}
		else {
			return colour.Multiply(0.5, 0.5, 0.5, 1);
		}
	}
	else
	{
		if ((abs(int(1.5 * x)) % 2) == (abs(int(1.5 * y))% 2)) {
			return colour.Multiply(0.5, 0.5, 0.5, 1);
		}
		else {
			return colour.Multiply(1.5, 1.5, 1.5, 1);
		}
	}
}

Point RayPlane::NormalAtPoint(Point point)
{
	return normal.Scale(-1);
}
