#pragma once

#include "RubiksCube.h"
#include "../Engine3D/scene.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

RubiksCube::RubiksCube() : Game()
{
}

RubiksCube::RubiksCube(float angle ,float relationWH, float near1, float far1) : Game(angle,relationWH,near1,far1)
{ 	
}

RubiksCube::~RubiksCube(void)
{
}

void RubiksCube::Init()
{		
    Game::Init();
    InitializeCube();
}

void RubiksCube::InitializeCube()
{
	int indx = 0;
	if (cube_size % 2 == 1)
		for (int i = -cube_size / 2; i <= cube_size / 2; i++)
			for (int j = -cube_size / 2; j <= cube_size / 2; j++)
				for (int k = -cube_size / 2; k <= cube_size / 2; k++)
				{
					AddShape(Cube,-1,TRIANGLES);
					SetShapeTex(indx,0);
					shapes[indx]->MyScale(glm::vec3(0.5f));
					shapes[indx]->MyTranslate(glm::vec3(i, j, k), 0);
                    int normalized_i= i + (cube_size / 2); 
                    int normalized_j = j + (cube_size / 2);
                    int normalized_k = k + (cube_size / 2);
                    cube[normalized_i][normalized_j][normalized_k] = shapes[indx];
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
					int normalized_i= i + (cube_size / 2); 
                    int normalized_j = j + (cube_size / 2);
                    int normalized_k = k + (cube_size / 2);
                    cube[normalized_i][normalized_j][normalized_k] = shapes[indx];
					indx++;
				}
		
		for (int i = 0; i < 3; i++)
			for (int j = 0; i < cube_size; i++)
				walls_angles[i][j] = 0.0f;
		
	
	MoveCamera(0,zTranslate,5*cube_size);
	rotation_angle = -45.0f;
	chosen_wall_to_rotate = 0;
}

void RubiksCube::DevideRotationAngle() 
{ 
	if (rotation_angle > 0)
		rotation_angle = glm::max(rotation_angle / 2, 1.40625f);
	else
		rotation_angle = glm::min(rotation_angle / 2, -1.40625f);
}

void RubiksCube::MultiplyRotationAngle() 
{ 
	if (rotation_angle > 0)
		rotation_angle = glm::min(rotation_angle * 2, 180.0f);
	else
		rotation_angle = glm::max(rotation_angle * 2, -180.0f);

}


// Wall index: 
// 	0             is BACK Wall
//  cube_size - 1 is FRONT Wall
void RubiksCube::RotateFrontToBackWall(int wall_indx, float angle)
{
	if (!CanRotate(FRONT_TO_BACK))
		return;
	
	vector<vector<Shape*>> wall;
	
	for (int i = 0; i < cube_size; i++)
	{
		vector<Shape*> row;
		for (int j = 0; j < cube_size; j++)
		{
			cube[i][j][wall_indx]->RotateRelative(angle, glm::vec3(0,0,1));
			row.push_back(cube[i][j][wall_indx]);
		}
		wall.push_back(row);
	}

	if (UpdateWallAngle(FRONT_TO_BACK, wall_indx, angle))
	{
		vector<vector<Shape*>> rotated_wall = RotateWall(wall, angle > 0, IsWallIn180DegreeAngle(FRONT_TO_BACK, wall_indx));
		
		for (int i = 0; i < cube_size; i++)
			for (int j = 0; j < cube_size; j++)
				cube[i][j][wall_indx] = rotated_wall[i][j];
		NormalizeWallAngle(FRONT_TO_BACK, wall_indx);
	}
	
}


// Wall index: 
// 	0             is BOTTOM Wall
//  cube_size - 1 is TOP Wall
void RubiksCube::RotateBottomToTopWall(int wall_indx, float angle)
{
	if (!CanRotate(BOTTOM_TO_TOP))
		return;
	
	vector<vector<Shape*>> wall;
	for (int i = 0; i < cube_size; i++)
	{
		vector<Shape*> row;
		for (int k = 0; k < cube_size; k++)
		{
			cube[i][wall_indx][k]->RotateRelative(angle, glm::vec3(0,1,0));
			row.push_back(cube[i][wall_indx][k]);
		}
		wall.push_back(row);
	}	

	if (UpdateWallAngle(BOTTOM_TO_TOP, wall_indx, angle))
	{
		vector<vector<Shape*>> rotated_wall = RotateWall(wall, angle < 0, IsWallIn180DegreeAngle(BOTTOM_TO_TOP, wall_indx));
		for (int i = 0; i < cube_size; i++)
			for (int k = 0; k < cube_size; k++)
				cube[i][wall_indx][k] = rotated_wall[i][k];
		NormalizeWallAngle(BOTTOM_TO_TOP, wall_indx);

	}
}

// Wall index: 
// 	0             is LEFT Wall
//  cube_size - 1 is RIGHT Wall
void RubiksCube::RotateRightToLeftWall(int wall_indx, float angle)
{
	if (!CanRotate(RIGHT_TO_LEFT))
		return;

	vector<vector<Shape*>> wall;

	for (int j = 0; j < cube_size; j++)
	{
		vector<Shape*> row;
		for (int k = 0; k < cube_size; k++)
		{
			cube[wall_indx][j][k]->RotateRelative(angle, glm::vec3(-1,0,0));
			row.push_back(cube[wall_indx][j][k]);
		}
		wall.push_back(row);
	}

	if (UpdateWallAngle(RIGHT_TO_LEFT, wall_indx, angle))
	{
		vector<vector<Shape*>> rotated_wall = RotateWall(wall, angle < 0, IsWallIn180DegreeAngle(RIGHT_TO_LEFT, wall_indx));
		for (int j = 0; j < cube_size; j++)
			for (int k = 0; k < cube_size; k++){
				cube[wall_indx][j][k] = rotated_wall[j][k];
			}
		NormalizeWallAngle(RIGHT_TO_LEFT, wall_indx);

	}
}

vector<vector<Shape*>> RubiksCube::RotateWall(vector<vector<Shape*>> wall, bool clockwise, bool is_180_angle){
	int num_rotations = 1;
	if (!clockwise)
		num_rotations = 3;
	if (is_180_angle)
		num_rotations = 2;
	

	vector<vector<Shape*>> tmp = wall;

	for (int n = 0; n < num_rotations; n++)
	{
		vector<vector<Shape*>> rotated_wall;
		for (int i = cube_size - 1; i >= 0; i--)
		{
			vector<Shape*> rotated_row;
			for (int j = 0; j < cube_size; j++)
			{
				rotated_row.push_back(tmp[j][i]);
			}
			rotated_wall.push_back(rotated_row);
			
		}
		tmp = rotated_wall;
	}
	return tmp;
}


bool RubiksCube::UpdateWallAngle(Direction dir, int wall_indx, float angle)
{ 
	walls_angles[dir][wall_indx] = walls_angles[dir][wall_indx] + angle;
	return IsWallIn90DegreeAngle(dir, wall_indx);
}

void RubiksCube::NormalizeWallAngle(Direction dir, int wall_indx)
{
	walls_angles[dir][wall_indx] = std::fmod(walls_angles[dir][wall_indx], 90);

}

bool RubiksCube::IsWallIn90DegreeAngle(Direction dir, int wall_indx)
{
	return std::fmod(walls_angles[dir][wall_indx], 90) == 0;
}

bool RubiksCube::IsWallIn180DegreeAngle(Direction dir, int wall_indx)
{
	return std::fmod(walls_angles[dir][wall_indx], 180) == 0;
}


bool RubiksCube::CanRotate(Direction dir)
{
    switch (dir)
	{
	case FRONT_TO_BACK:
		return AreAllWallsAngleInDirectionZero(BOTTOM_TO_TOP) && AreAllWallsAngleInDirectionZero(RIGHT_TO_LEFT);
	case BOTTOM_TO_TOP:
		return AreAllWallsAngleInDirectionZero(FRONT_TO_BACK) && AreAllWallsAngleInDirectionZero(RIGHT_TO_LEFT);
	default:
		return AreAllWallsAngleInDirectionZero(FRONT_TO_BACK) && AreAllWallsAngleInDirectionZero(BOTTOM_TO_TOP);
	}
}

bool RubiksCube::AreAllWallsAngleInDirectionZero(Direction dir)
{
	for (int i = 0; i < cube_size; i++)
		if (walls_angles[dir][i] != 0.0f)
			return false;
    return true;
}

int RubiksCube::RandomaizeWallIndx(){
	return rand() % cube_size;
}
int RubiksCube::RandomaizeAction(){
	return rand() % 3;
}

void RubiksCube::RandomizeCubeStep()
{
	ofstream file;
	if (random_step_number == 1)
	{
		file.open("../res/mixer_tracking.txt");
	}
	else
		file.open("../res/mixer_tracking.txt", ios::app);

	float angle = -90.0f;

	int wall_indx = RandomaizeWallIndx();
	int action = RandomaizeAction();
	ChooseWallToRotate(wall_indx);
	string direction;
	switch (action)
	{
	case FRONT_TO_BACK:
		RotateFrontToBackWall(wall_indx, angle);
		direction="FRONT_TO_BACK";
		break;
	case RIGHT_TO_LEFT:
		RotateRightToLeftWall(wall_indx, angle);
		direction="RIGHT_TO_LEFT";
		break;
	case BOTTOM_TO_TOP:
		RotateBottomToTopWall(wall_indx, angle);
		direction="BOTTOM_TO_TOP";
		break;
	}
	file << "Step "<< random_step_number <<" - Rotate wall "<< wall_indx << " in direction "<< direction << " " << glm::abs(angle) << " degrees clockwise." << endl;
	cout << "Step "<< random_step_number <<" - Rotate wall "<< wall_indx << " in direction "<< direction << " " << glm::abs(angle) << " degrees clockwise." << endl;

	file.close();
}

void RubiksCube::UpdateRandomStep() {
	if(random_step_number == 100){
		Unlock();
		random_step_number = 1;
	}
	else
		random_step_number++;
}


void RubiksCube::Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode){
	if (IsLocked())
	{
		RandomizeCubeStep();
		UpdateRandomStep();
	}
	Scene::Draw(shaderIndx,cameraIndx,buffer,toClear,debugMode);
}
