#include "Image.h"
#include <stdlib.h>
#include <iostream>
unsigned char* Image::ToCharArray()
{
	std::vector<unsigned char> charDataVectored;
	for (auto val : data) {
			charDataVectored.push_back(val.red);
			charDataVectored.push_back(val.green);
			charDataVectored.push_back(val.blue);
			charDataVectored.push_back(val.intensity);
	}
	unsigned char* charData;
	charData = (unsigned char*)malloc(charDataVectored.size() * sizeof(unsigned char));
	std::copy(charDataVectored.begin(), charDataVectored.end(), charData);
    return charData;
}
