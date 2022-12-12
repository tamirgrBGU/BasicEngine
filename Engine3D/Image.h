#pragma once
#include "Colour.h"
#include <vector>
class Image
{
public:
	Image(int width, int height) : width(width), height(height) {};
	std::vector<Colour> data;
	unsigned char* ToCharArray();
private:
	int width;
	int height;
};

