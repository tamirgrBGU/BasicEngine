#include "Colour.h"
#include <utility>

Colour Colour::Add(Colour colour)
{
	return Colour(std::min((int)red+(int)colour.red,255), 
		std::min((int)green+(int)colour.green,255),
		std::min((int)blue+(int)colour.blue,255),
		std::min((int)intensity+(int)colour.intensity,255));
}

Colour Colour::Multiply(double rScale, double gScale, double bScale, double iScale)
{
	return Colour(std::min((int)red*rScale,255.0),
		std::min((int)green*gScale,255.0),
		std::min((int)blue*bScale,255.0),
		std::min((int)intensity*iScale,255.0));
}
