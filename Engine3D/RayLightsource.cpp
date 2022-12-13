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
}

Colour Directional::LightIntersection(Intersection* hit)
{
    Colour colour = Colour(0, 0, 0, 0);

    RayObject* rayObject = hit->object;
    Colour objectColour = rayObject->ColourAtPoint(hit->point);
    Point normal = rayObject->NormalAtPoint(hit->point);

    // Diffuse lighting
    double cosAngle = normal.Dot(direction.Scale(-1));
    if (cosAngle > 0) {
        colour = colour.Add(objectColour.
                        Multiply(cosAngle, cosAngle, cosAngle, 1).
                        Multiply(rScale,gScale,bScale,iScale));
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
}

Colour Spotlight::LightIntersection(Intersection* hit)
{
    Colour colour = Colour(0, 0, 0, 0);

    RayObject* rayObject = hit->object;
    Colour objectColour = rayObject->ColourAtPoint(hit->point);
    Point normal = rayObject->NormalAtPoint(hit->point);

    // ADD CHECK THAT IN SPOTLIGHT

    // Diffuse lighting
    double cosAngle = normal.Dot(direction.Scale(-1));
    if (cosAngle > 0) {
        colour = colour.Add(objectColour.
            Multiply(cosAngle, cosAngle, cosAngle, 1).
            Multiply(rScale, gScale, bScale, iScale));
    }

    // Specular lighting
    double un = direction.Dot(normal);
    if (un < 0) {
        Point reflected = direction.Sub(normal.Scale(2 * un));
        double cosAlpha = reflected.Dot(direction.Scale(-1));
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
    return new Ray(point, origin.Sub(point));
}

