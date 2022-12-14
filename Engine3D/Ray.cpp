#include "Ray.h"
#include "RayObject.h"
#include "Intersection.h"

Ray Ray::ReflectOff(Intersection* hit)
{
    RayObject* rayObject = hit->object;
    Point normal = rayObject->NormalAtPoint(hit->point);
    double un = direction.Dot(normal);
    Point reflected = direction.Sub(normal.Scale(2 * un)).Normalised();

    return Ray(hit->point.Add(reflected.Scale(0.01)), reflected);
}

Ray Ray::RefractAt(Intersection* hit)
{
    double refract2;
    if (refractiveIndex == 1) {
        refract2 = 1.5;
    }
    else {
        refract2 = 1;
    }
    RayObject* rayObject = hit->object;
    Point normal = rayObject->NormalAtPoint(hit->point);
    double iOmega = acos(fabs(direction.Dot(normal)));
    double rOmega = asin(sin(iOmega) * refractiveIndex / refract2);
    double nTerm = cos(iOmega) * refractiveIndex / refract2 - cos(rOmega);
    Point tDirection = normal.Scale(nTerm).Sub(direction.Scale(-refractiveIndex / refract2)).Normalised();

    Ray outRay = Ray(hit->point.Add(tDirection.Scale(0.01)), tDirection);
    outRay.refractiveIndex = refract2;
    return outRay;
}


