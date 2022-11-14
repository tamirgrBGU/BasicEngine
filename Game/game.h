#pragma once
#include "scene.h"

class Game : public Scene
{
public:
	
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	unsigned char* Grayscale(int width, int height, unsigned char* data);
	unsigned char* EdgeDetection(int width, int height, unsigned char* data);
	unsigned char* Halftones(int width, int height, unsigned char* data);
	unsigned char* FSAlgorithm(int width, int height, unsigned char* data);
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
};

