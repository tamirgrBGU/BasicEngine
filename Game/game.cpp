#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	AddTexture("../res/textures/plane.png",false);


	int cube_size = 3;
	int indx = 0;
	if (cube_size % 2 == 1)
		for (int i = -cube_size / 2; i <= cube_size / 2; i++)
			for (int j = -cube_size / 2; j <= cube_size / 2; j++)
				for (int k = -cube_size / 2; k <= cube_size / 2; k++)
				{
					AddShape(Cube,-1,TRIANGLES);
					SetShapeTex(indx,0);
					shapes[indx]->MyScale(glm::vec3(0.5f));
					//shapes[indx]->MyTranslate(glm::vec3(i,j,0), 0);
					if (k == -1)
					{
						shapes[indx]->MyTranslate(glm::vec3(i,j,k), 0);
					}
					
					indx++;
				}
	else
		for (int i = -cube_size / 2; i < cube_size / 2; i++)
			for (int j = -cube_size / 2; j < cube_size / 2; j++)
				for (int k = -cube_size / 2; k < cube_size / 2; k++)
				{
					AddShape(Cube,-1,TRIANGLES);
					SetShapeTex(indx,0);
					shapes[indx]->MyScale(glm::vec3(0.5f));
					shapes[indx]->MyTranslate(glm::vec3(i + 0.5,j + 0.5,k + 0.5), 0);
					indx++;
				}
	
	
	

	pickedShape = 0;
	
	MoveCamera(0,zTranslate,3*cube_size);
	pickedShape = -1;

	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
