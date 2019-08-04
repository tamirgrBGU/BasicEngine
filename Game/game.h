#pragma once
#include "scene.h"

class Game : public Scene
{
	float count;
public:
	
	Game(void);
	Game(glm::vec3 position,float angle,float near, float far);
	void Init();
	void addShape(int type,int parent,unsigned int mode);
//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
};

