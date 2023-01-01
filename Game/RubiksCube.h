#pragma once

#include "game.h"
#include <iostream>
#include <vector>
#include "../Engine3D/shape.h"

using namespace std;

enum Direction
{
	FRONT_TO_BACK,
	RIGHT_TO_LEFT,
	BOTTOM_TO_TOP
};

class RubiksCube : public Game
{
public:
	RubiksCube();
	RubiksCube(float angle, float relationWH, float near, float far);
	~RubiksCube(void);
	void Init();
	static const int cube_size = 3;


	virtual void Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode);

	void InitializeCube();
	void RotateFrontToBackWall(int wall_indx, float angle);
	void RotateRightToLeftWall(int wall_indx, float angle);
	void RotateBottomToTopWall(int wall_indx, float angle);
	void FlipRotation() { rotation_angle *= -1; }
	float GetRotationAngle() { return rotation_angle; }

	bool UpdateWallAngle(Direction dir, int wall_indx, float angle);
	void NormalizeWallAngle(Direction dir, int wall_indx);
	bool IsWallIn90DegreeAngle(Direction dir, int wall_indx);
	bool IsWallIn180DegreeAngle(Direction dir, int wall_indx);
	bool CanRotate(Direction dir);
	bool AreAllWallsAngleInDirectionZero(Direction dir);
	void DevideRotationAngle();
	void MultiplyRotationAngle();
	void ChooseWallToRotate(int wall_indx) {chosen_wall_to_rotate = glm::min(wall_indx, cube_size - 1);}
	int GetChosenWallToRotate() { return chosen_wall_to_rotate; }
	void RandomizeCubeStep();
	bool IsLocked() {return lock;}
	void UpdateRandomStep();
	void Lock() {lock = true;}
	void Unlock() {lock=false;}


protected:
	vector<vector<Shape *>> RotateWall(vector<vector<Shape *>> wall, bool clockwise, bool is_180_angle);

	Shape *cube[cube_size][cube_size][cube_size];
	float walls_angles[3][cube_size];
	float rotation_angle;
	int chosen_wall_to_rotate;

	bool lock = false;
	int random_step_number = 1;
private:
	int RandomaizeWallIndx();
	int RandomaizeAction();
};