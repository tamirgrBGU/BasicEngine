#include "RayObject.h"
#include "Intersection.h"
#include "Ray.h"
#include <math.h>
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
