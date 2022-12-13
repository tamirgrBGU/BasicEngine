#pragma once
class Colour
{
public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char intensity;
	Colour(unsigned char red,unsigned char green,unsigned char blue, unsigned char intensity) : 
		red(red), 
		green(green), 
		blue(blue),
		intensity(intensity)
	{};
	Colour Add(Colour colour);
	Colour Multiply(double rScale, double gScale, double bScale, double iScale);
};

