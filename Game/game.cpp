#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.c>

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

	std::string fileName = "../res/textures/lena256.jpg";
	//std::string fileName = "../res/textures/box0.bmp";
	int width = 256;
	int height = 256;
	int numComponents = 4;
	
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	unsigned char* grayscaled = Grayscale(width,height,data);
	unsigned char* edges = EdgeDetection(width,height, grayscaled);
	unsigned char* halftoned = Halftones(width,height,data);
	unsigned char* fsalgo = FSAlgorithm(width,height,data);

	AddTexture(width, height, grayscaled);
	AddTexture(width, height, edges);
	AddTexture(width, height, halftoned);
	AddTexture(width, height, fsalgo);

	free(grayscaled);
	free(edges);

	AddShape(Plane,-1,TRIANGLES);
	
	pickedShape = 0;
	
	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

unsigned char* Game::Grayscale(int width, int height, unsigned char* data)
{
	std::vector<unsigned char> grayscaledVector;
	for (int i = 0; i < width * height*4; i=i+4) {
		unsigned char avg = (data[i] + data[i + 1] + data[i + 2]) / 3;
		grayscaledVector.push_back(avg);
		grayscaledVector.push_back(avg);
		grayscaledVector.push_back(avg);
		grayscaledVector.push_back(data[i+3]);
	}
	unsigned char* grayscaled;
	grayscaled = (unsigned char*)malloc(grayscaledVector.size() * sizeof(unsigned char));
	std::copy(grayscaledVector.begin(), grayscaledVector.end(), grayscaled);
	return grayscaled;
}

unsigned char* Game::EdgeDetection(int width, int height, unsigned char* data)
{
	std::vector<unsigned char> gradientIntensitiesVector;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width ; col++) {
			int dx = data[row * width * 4 + col * 4] - data[row * width * 4 + (col + 1) * 4];
			int dy = data[row * width * 4 + col * 4] - data[(row+1) * width * 4 + col * 4];
			int gradient = sqrt(dx * dx + dy * dy);
			gradientIntensitiesVector.push_back(gradient);
			gradientIntensitiesVector.push_back(gradient);
			gradientIntensitiesVector.push_back(gradient);
			gradientIntensitiesVector.push_back(data[row * width * 4 + col * 4 + 3]);
		}
	}
	unsigned char* gradientIntensities;
	gradientIntensities = (unsigned char*)malloc(gradientIntensitiesVector.size() * sizeof(unsigned char));
	std::copy(gradientIntensitiesVector.begin(), gradientIntensitiesVector.end(), gradientIntensities);
	return gradientIntensities;
}

unsigned char* Game::Halftones(int width, int height, unsigned char* data)
{
	return data;
}

unsigned char* Game::FSAlgorithm(int width, int height, unsigned char* data)
{
	return data;
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


