#include "game.h"
#include "glad/include/glad/glad.h"
#include <iostream>
#include <vector>
#include "../Engine3D/shape.h"



class RubiksCube : public Game
{
public:
	
	RubiksCube(int size);
	RubiksCube(float angle,float relationWH,float near, float far, int size);
	~RubiksCube(void);
	static const int cube_size=3;

	void initialize_walls();
protected:
    int front_to_back_walls_indx[cube_size][cube_size];
	int right_to_left_walls_indx[cube_size][cube_size];
	int bottom_to_up_walls_indx[cube_size][cube_size];
};