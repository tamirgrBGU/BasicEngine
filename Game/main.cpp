#include "InputManager.h"
#include "glm\glm.hpp"

	int main(int argc,char *argv[])
{

	const int DISPLAY_WIDTH = 1600;
	const int DISPLAY_HEIGHT = 800;

	const float zFar = 100.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;

	Game *scn = new Game(glm::vec3(0.0f, 0.0f, -1.0f), CAM_ANGLE, zNear,zFar);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
	
	scn->Init();

	scn->AddShader("../res/shaders/pickingShader");	
	scn->AddShader("../res/shaders/basicShader");
	
	scn->AddTexture("../res/textures/box0.bmp",false);

	display.SetScene(scn);

	while(!display.CloseWindow())
	{
		scn->Draw(1,0,BACK,true,false);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();
		
	}
	delete scn;
	return 0;
}
