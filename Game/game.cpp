#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.c>
#include <numbers>
#include <iostream>
#include <fstream>
#include "RayScene.h"
#include "RayObject.h"
#include <stack>
#include <queue>
using namespace std;
# define PI           3.14159265358979323846  /* pi */
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

RayScene Game::LoadSceneFile(const std::string& fileName) {
	RayScene rayScene = RayScene();

	ifstream sceneFile;

	sceneFile.open(fileName);

	string eyeLine;
	float xEye, yEye, zEye, bonusEye;
	getline(sceneFile, eyeLine);
	sscanf(eyeLine.c_str(), "e %f %f %f %f", &xEye, &yEye, &zEye, &bonusEye);
	rayScene.SetCamera(Point(xEye, yEye, zEye));
	if (bonusEye == 1) {
		samples = 4;
	}

	string ambientLine;
	float rAmbient, gAmbient, bAmbient, aAmbient;
	getline(sceneFile, ambientLine);
	sscanf(ambientLine.c_str(), "a %f %f %f %f", &rAmbient, &gAmbient, &bAmbient, &aAmbient);
	rayScene.AddRayLightsource(new Ambient(rAmbient, gAmbient, bAmbient, aAmbient));

	string currentLine;
	char identifier;
	float arg1, arg2, arg3, arg4;
	int currentObjectToColour = 0;
	int currentLightToSetI = 1; // AMBIENT STARTED BEFORE
	std::queue<int> spotlightIndexes = queue<int>();
	int currentLightIndex = 1;
	while (getline(sceneFile, currentLine)) {
		sscanf(currentLine.c_str(), "%c %f %f %f %f", &identifier, &arg1, &arg2, &arg3, &arg4);
		switch (identifier) {
		case 'd':
			if (arg4 == 0.0) {
				// Directional
				rayScene.AddRayLightsource(new Directional(Point(arg1, arg2, arg3)));
			}
			else {
				// Spotlight
				rayScene.AddRayLightsource(new Spotlight(Point(arg1, arg2, arg3)));
				spotlightIndexes.push(currentLightIndex);
			}
			currentLightIndex++;
			break;
		case 'p':
			rayScene.SetLightsourceOrigin(spotlightIndexes.front(), Point(arg1, arg2, arg3));
			rayScene.SetLightsourceCutoffCosAlpha(spotlightIndexes.front(), arg4);
			spotlightIndexes.pop();
			break;
		case 'i':
			rayScene.SetLightsourceIntensity(currentLightToSetI, arg1, arg2, arg3, arg4);
			currentLightToSetI++;
			break;
		case 'o':
			if (arg4 > 0) {
				// Sphere
				rayScene.AddRayObject(new Sphere(Point(arg1, arg2, arg3), arg4));
			}
			else {
				// Plane
				rayScene.AddRayObject(new RayPlane(Point(arg1, arg2, arg3), arg4));
			}
			break;
		case 'r':
			if (arg4 > 0) {
				// Sphere
				Sphere* sphere = new Sphere(Point(arg1, arg2, arg3), arg4);
				sphere->MarkMirror();
				rayScene.AddRayObject(sphere);
			}
			else {
				// Plane
				RayPlane* plane = new RayPlane(Point(arg1, arg2, arg3), arg4);
				plane->MarkMirror();
				rayScene.AddRayObject(plane);
			}
			break;
		case 't':
			if (arg4 > 0) {
				// Sphere
				Sphere* sphere = new Sphere(Point(arg1, arg2, arg3), arg4);
				sphere->MarkTransparent();
				rayScene.AddRayObject(sphere);
			}
			else {
				// Plane
				RayPlane* plane = new RayPlane(Point(arg1, arg2, arg3), arg4);
				plane->MarkTransparent();
				rayScene.AddRayObject(plane);
			}
			break;
		case 'c':
			rayScene.ColourRayObject(currentObjectToColour, Colour(arg1*255, arg2*255, arg3*255, 255));
			rayScene.SetRayObjectShininess(currentObjectToColour, arg4);
			currentObjectToColour++;
			break;
		default:
			cout << "Invalid scene line!" << endl;
		}
	}

	sceneFile.close();

	return rayScene;
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");

	std::string fileName = "../res/textures/lena256.jpg";
	int width = 800;
	int height = 800;

	RayScene rayScene = LoadSceneFile("../scene.txt");
	
	unsigned char* data = rayScene.Render(width,height, samples, 5);
	AddTexture(width, height, data);
	free(data);
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
	int sobelXMatrix[3][3] = {	{1,0,-1}, 
											{2,0,-2},
											{1,0,-1} };

	int sobelYMatrix[3][3] = {	{1,2,1}, 
											{0,0,0},
											{-1,-2,-1} };

	int lowThreshold = 0.9 * 255;
// Make a 2d version of the data for easier handling.	
	std::vector<std::vector<int>> data2d(height, std::vector<int> (width));

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			data2d[row][col] = data[row * width * 4 + col * 4];
		}
	}

	std::vector<std::vector<int>> gradientMatrix(height, std::vector<int> (width));
	std::vector<std::vector<int>> edgesMatrix(height, std::vector<int> (width));

	// GradientDirectionMatrix in degrees.
	std::vector<std::vector<int>> gradientDirectionMatrix(height, std::vector<int> (width));

	int max_gradient = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			int sumX = 0;
			int sumY = 0;
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if (i == 0 && j == 0) {
						continue;
					}
					int x = row + i;
					int y = col + j;
					if (x < 0 || x > width - 1 || y < 0 || y > height - 1) {
						continue;
					}

					sumX = sumX + sobelXMatrix[i + 1][j + 1] * data2d[x][y];
					sumY = sumY + sobelYMatrix[i + 1][j + 1] * data2d[x][y];
				}
			}


			int gradient = sqrt(sumX * sumX + sumY * sumY);
			if (gradient > max_gradient) {
				max_gradient = gradient;
			}
			gradientMatrix[row][col] = gradient;

			int gradientDirection = atan2(sumY, sumX);
			gradientDirectionMatrix[row][col] = trunc(gradientDirection * 180 / PI + 0.5);
		}
	}
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (gradientMatrix[row][col] < lowThreshold) {
				edgesMatrix[row][col] = 0;
				continue;
			}

			int prev = 0;
			int next = 0;
			int direction = gradientDirectionMatrix[row][col];
			int intensity = gradientMatrix[row][col];
			// This section maps a direction and rounds it to the nearest cardinal direction to compare to the 
			// correct pixels and find the local maximum.
			// Direction East-West
			if (direction <= -157.5 || (direction >= -22.5 && direction <= 22.5) || (direction >= 157.5) ) {
				if (col + 1 < width && intensity >= gradientMatrix[row][col + 1]) {
					next = gradientMatrix[row][col + 1];
				}
				if (col != 0 && intensity >= gradientMatrix[row][col -1]) {
					prev = gradientMatrix[row][col - 1];
				}
			}
// Direction North West-South East
			if ((direction >= 22.5 && direction <= 67.5) || (direction >= -157.5 && direction <= -112.5) ) {
				if (col + 1 < width && row != 0 && intensity >= gradientMatrix[row - 1][col + 1]) {
					next = gradientMatrix[row - 1][col + 1];
				}
				if (col != 0 && row + 1 < height && intensity >= gradientMatrix[row + 1][col -1]) {
					prev = gradientMatrix[row + 1][col - 1];
				}
			}
// Direction North-South
			if ((direction >= 67.5 && direction <= 112.5) || (direction >= -112.5 && direction <= -67.5) ) {
				if (row + 1 < width && intensity >= gradientMatrix[row+1][col]) {
					next = gradientMatrix[row + 1][col];
				}
				if (row != 0 && intensity >= gradientMatrix[row-1][col]) {
					prev = gradientMatrix[row - 1][col];
				}
			}
// Direction North East-South West
			if ((direction >= 112.5 && direction <= 157.5) || (direction >= -67.5 && direction <= -22.5) ) {
				if (col + 1 < width && intensity >= gradientMatrix[row][col + 1]) {
					next = gradientMatrix[row][col + 1];
				}
				if (col != 0 && intensity >= gradientMatrix[row][col -1]) {
					prev = gradientMatrix[row][col - 1];
				}
			}
			
			if (intensity > prev && intensity > next) {
				edgesMatrix[row][col] = 255;
			}
			else {
				edgesMatrix[row][col] = 0;
			}
		}
	}

	std::ofstream outputFile;
	outputFile.open("../img4.txt");
	std::vector<unsigned char> gradientIntensitiesVector;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			outputFile << std::to_string(edgesMatrix[row][col] / 255) << ",";
			gradientIntensitiesVector.push_back(edgesMatrix[row][col]);
			gradientIntensitiesVector.push_back(edgesMatrix[row][col]);
			gradientIntensitiesVector.push_back(edgesMatrix[row][col]);
			gradientIntensitiesVector.push_back(data[row * width * 4 + col * 4 + 3]);
		}
	}
	outputFile.close();
	
	unsigned char* gradientIntensities;
	gradientIntensities = (unsigned char*)malloc(gradientIntensitiesVector.size() * sizeof(unsigned char));
	std::copy(gradientIntensitiesVector.begin(), gradientIntensitiesVector.end(), gradientIntensities);
	return gradientIntensities;
}

unsigned char* Game::Halftones(int width, int height, unsigned char* data)
{
	const int MAX_INTENSITY = 255;
	const int NUM_INTENSITIES = 4;
	// Make a 2d version of the data for easier handling.	
	std::vector<std::vector<double>> data2d(height, std::vector<double>(width));

	int max = 0;
	int min = 255;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			data2d[row][col] = data[row * width * 4 + col * 4];
			if (data2d[row][col] > max) {
				max = data2d[row][col];
			}
			if (data2d[row][col] < min) {
				min = data2d[row][col];
			}
		}
	}

	// Tried improving contrast, didn't seem to do much.
	double factor = 255.0 / (double)(max-min);
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			data2d[row][col] = trunc((double)(data2d[row][col] - min) * factor);
		}
			
	}


	std::vector<std::vector<int>> halftonedMatrix(height, std::vector<int>(width));

	for (int row = 0; row < height-1; row+=2) {
		for (int col = 0; col < width-1; col+=2) {
			double average = (data2d[row][col] + data2d[row][col + 1] + data2d[row + 1][col] + data2d[row + 1][col + 1])/4.0;
			int intensity = trunc(((double)average / (double)MAX_INTENSITY) * (double)NUM_INTENSITIES + 0.5);
			switch (intensity) {
			case 0:
				halftonedMatrix[row][col] = 0;
				halftonedMatrix[row][col+1] = 0;
				halftonedMatrix[row+1][col] = 0;
				halftonedMatrix[row+1][col+1] = 0;
				break;
			case 1:
				halftonedMatrix[row][col] = 255;
				halftonedMatrix[row][col+1] = 0;
				halftonedMatrix[row+1][col] = 0;
				halftonedMatrix[row+1][col+1] = 0;
				break;
			case 2:
				halftonedMatrix[row][col] = 255;
				halftonedMatrix[row][col+1] = 0;
				halftonedMatrix[row+1][col] = 0;
				halftonedMatrix[row+1][col+1] = 255;
				break;
			case 3:
				halftonedMatrix[row][col] = 255;
				halftonedMatrix[row][col+1] = 255;
				halftonedMatrix[row+1][col] = 0;
				halftonedMatrix[row+1][col+1] = 255;
				break;
			case 4:
				halftonedMatrix[row][col] = 255;
				halftonedMatrix[row][col+1] = 255;
				halftonedMatrix[row+1][col] = 255;
				halftonedMatrix[row+1][col+1] = 255;
				break;
			}
		}
	}
	std::ofstream outputFile;
	outputFile.open("../img5.txt");

	std::vector<unsigned char> halftonedVector;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			outputFile << std::to_string((int)(halftonedMatrix[row][col] / 255)) << ",";
			halftonedVector.push_back(halftonedMatrix[row][col]);
			halftonedVector.push_back(halftonedMatrix[row][col]);
			halftonedVector.push_back(halftonedMatrix[row][col]);
			halftonedVector.push_back(data[row * width * 4 + col * 4 + 3]);
		}
	}
	outputFile.close();

	unsigned char* halftoned;
	halftoned = (unsigned char*)malloc(halftonedVector.size() * sizeof(unsigned char));
	std::copy(halftonedVector.begin(), halftonedVector.end(), halftoned);
	return halftoned;
}

unsigned char* Game::FSAlgorithm(int width, int height, unsigned char* data)
{
	const int NUM_INTENSITIES = 15;
	const int MAX_INTENSITY = 255;
	// Make a 2d version of the data for easier handling.	
	std::vector<std::vector<double>> data2d(height, std::vector<double>(width));

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			data2d[row][col] = data[row * width * 4 + col * 4];
		}
	}

	std::vector<std::vector<int>> fsMatrix(height, std::vector<int>(width));

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			int intensity = trunc(((double)data2d[row][col] / (double)MAX_INTENSITY) * (double)NUM_INTENSITIES + 0.5);
			fsMatrix[row][col] = trunc((double)intensity * ((double)MAX_INTENSITY / (double)NUM_INTENSITIES));
			double error = data2d[row][col] - fsMatrix[row][col];
			if (col + 1 < width) {
				data2d[row][col + 1] += error * 7.0 / 16.0;
			}
			if (col != 0 && row + 1 < height) {
				data2d[row + 1][col - 1] += error * 3.0 / 16.0;
			}
			if (row + 1 < height) {
				data2d[row + 1][col] += error * 5.0 / 16.0;
			}
			if (col + 1 < width && row + 1 < height) {
				data2d[row + 1][col + 1] += error * 1.0 / 16.0;
			}
		}
	}

	std::ofstream outputFile;
	outputFile.open("../img6.txt");
	std::vector<unsigned char> fsVector;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			outputFile << std::to_string((int)trunc(((double)fsMatrix[row][col] / (double)MAX_INTENSITY) * (double)NUM_INTENSITIES + 0.5)) << ",";
			fsVector.push_back(fsMatrix[row][col]);
			fsVector.push_back(fsMatrix[row][col]);
			fsVector.push_back(fsMatrix[row][col]);
			fsVector.push_back(data[row * width * 4 + col * 4 + 3]);
		}
	}
	outputFile.close();

	unsigned char* fsalgo;
	fsalgo = (unsigned char*)malloc(fsVector.size() * sizeof(unsigned char));
	std::copy(fsVector.begin(), fsVector.end(), fsalgo);
	return fsalgo;
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


