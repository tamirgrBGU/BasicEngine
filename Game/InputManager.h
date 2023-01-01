#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include "RubiksCube.h"


	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
			scn->Picking((int)x2,(int)y2);
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		scn->MyTranslate(glm::vec3(0,0,xoffset),0);
		
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		RubiksCube *scn = (RubiksCube*)glfwGetWindowUserPointer(window);

		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			float angle = scn->GetRotationAngle();
			switch (key)
			{
				case GLFW_KEY_R:
					scn->RotateRightToLeftWall(scn->cube_size - 1, angle);
					break;
				case GLFW_KEY_L:
					scn->RotateRightToLeftWall(0,angle);
					break;
				case GLFW_KEY_U:
					scn->RotateBottomToTopWall(scn->cube_size - 1, angle);
					break;
				case GLFW_KEY_D:
					scn->RotateBottomToTopWall(0, angle);
					break;
				case GLFW_KEY_B:
					scn->RotateFrontToBackWall(0, angle);
					break;
				case GLFW_KEY_F:
					scn->RotateFrontToBackWall(scn->cube_size - 1, angle);
					break;
				case GLFW_KEY_Z:
					scn->DevideRotationAngle();
					break;
				case GLFW_KEY_A:
					scn->MultiplyRotationAngle();
					break;
				case GLFW_KEY_0:
				case GLFW_KEY_1:
				case GLFW_KEY_2:
				case GLFW_KEY_3:
				case GLFW_KEY_4:
				case GLFW_KEY_5:
				case GLFW_KEY_6:
				case GLFW_KEY_7:
				case GLFW_KEY_8:
				case GLFW_KEY_9:
					scn->ChooseWallToRotate(key - GLFW_KEY_0);
					break;
				case GLFW_KEY_Q:
					scn->RotateRightToLeftWall(scn->GetChosenWallToRotate(), angle);
					break;
				case GLFW_KEY_W:
					scn->RotateBottomToTopWall(scn->GetChosenWallToRotate(), angle);
					break;
				case GLFW_KEY_E:
					scn->RotateFrontToBackWall(scn->GetChosenWallToRotate(), angle);
					break;
				case GLFW_KEY_M:
					scn->Lock();
					break;
				case GLFW_KEY_SPACE:
					if(!scn->IsActive())
						scn->Activate();	
					else
						scn->FlipRotation();
					break;
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;
				/*case GLFW_KEY_SPACE:
					if(scn->IsActive())
						scn->Deactivate();
					else
						scn->Activate();
				break;*/
				case GLFW_KEY_UP:
					scn->MoveCamera(0,scn->zTranslate,0.4f);
					for (int i = 0; i < 27; i++)
					{
						scn->RotateShape(i, glm::vec3(0,0,1), 45.0f);
					}
					
					
					break;
				case GLFW_KEY_DOWN:
					//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
					//cout<< "down: "<<endl;
					scn->MoveCamera(0,scn->zTranslate,-0.4f);
					for (int i = 0; i < 27; i++)
					{
						scn->RotateShape(i, glm::vec3(0,1,0), 45.0f);
					}
					break;

			default:
				break;
			}
		}
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->UpdatePosition((float)xpos,(float)ypos);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->Resize(width,height);
		
	}

	void Init(Display &display)
	{
		display.AddKeyCallBack(key_callback);
		display.AddMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.AddResizeCallBack(window_size_callback);
	}
