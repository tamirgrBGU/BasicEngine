#include "RayLightsource.h"
#include "RayObject.h"
#include <math.h>

Ray* Ambient::ConstructLightRay(Point point)
{
    return nullptr;
}

Colour Ambient::LightIntersection(Intersection* hit)
{
    RayObject* rayObject = hit->object;
    Colour objectColour = rayObject->ColourAtPoint(hit->point);
    return objectColour.Multiply(rScale, gScale, bScale, iScale);
    //return Colour(0, 0, 0, 0);
}

Colour Directional::LightIntersection(Intersection* hit)
{
    Colour colour = Colour(0, 0, 0, 0);

    RayObject* rayObject = hit->object;
    Colour objectColour = rayObject->ColourAtPoint(hit->point);
    Point normal = rayObject->NormalAtPoint(hit->point);
    std::vector<double> diffuse = rayObject->DiffuseAtPoint(hit->point);
    // Diffuse lighting
    double cosAngle = normal.Dot(direction.Scale(-1));
    if (cosAngle > 0) {
        colour = colour.Add(objectColour.
                        Multiply(cosAngle, cosAngle, cosAngle, 1).
                        Multiply(rScale,gScale,bScale,iScale).
                        Multiply(diffuse[0],diffuse[1],diffuse[2],diffuse[3]));
    }

    // Specular lighting
    double un = direction.Dot(normal);
    if (un < 0) {
		Point reflected = direction.Sub(normal.Scale(2*un));
		double cosAlpha = reflected.Dot(direction.Scale(-1));
        if (cosAlpha > 0) {
			double cosAlphaPow = pow(cosAlpha, rayObject->shininess);
			colour = colour.Add(Colour(255,255,255,255).
								Multiply(cosAlphaPow, cosAlphaPow, cosAlphaPow, 1).
								Multiply(0.7,0.7,0.7,1).
                                Multiply(rScale,gScale,bScale,iScale));
        }
    }

    return colour;
    //return Colour(0, 0, 0, 0);
}

Colour Spotlight::LightIntersection(Intersection* hit)
{
    Colour colour = Colour(0, 0, 0, 0);

    RayObject* rayObject = hit->object;
    Colour objectColour = rayObject->ColourAtPoint(hit->point);
    Point normal = rayObject->NormalAtPoint(hit->point);
    std::vector<double> diffuse = rayObject->DiffuseAtPoint(hit->point);

    // ADD CHECK THAT IN SPOTLIGHT
    Point V = hit->point.Sub(origin).Normalised();
    double cosOmega = direction.Dot(V);
    if (cosOmega <= cutoffCosAlpha) {
        return colour;
    }

    // Diffuse lighting
    double cosAngle = normal.Dot(V.Scale(-1));
    if (cosAngle > 0) {
        colour = colour.Add(objectColour.
            Multiply(cosAngle, cosAngle, cosAngle, 1).
            Multiply(rScale, gScale, bScale, iScale).
            Multiply(diffuse[0],diffuse[1],diffuse[2],diffuse[3]));
    }

    // Specular lighting
    double un = V.Dot(normal);
    if (un < 0) {
        Point reflected = V.Sub(normal.Scale(2 * un));
        double cosAlpha = reflected.Dot(V.Scale(-1));
        if (cosAlpha > 0) {
            double cosAlphaPow = pow(cosAlpha, rayObject->shininess);
            colour = colour.Add(Colour(255, 255, 255, 255).
                Multiply(cosAlphaPow, cosAlphaPow, cosAlphaPow, 1).
                Multiply(0.7, 0.7, 0.7, 1).
                Multiply(rScale, gScale, bScale, iScale));
        }
    }

    return colour;
}

Ray* Directional::ConstructLightRay(Point point)
{
    return new Ray(point, direction.Scale(-1));
}

Ray* Spotlight::ConstructLightRay(Point point)
{
    return new Ray(point, origin.Sub(point).Normalised());
}

void Spotlight::SetCutoffCosAlpha(double cosAlpha)
{
    cutoffCosAlpha = cosAlpha;
}

double Spotlight::DistanceToPoint(Point point)
{
    return point.Distance(origin);
}
