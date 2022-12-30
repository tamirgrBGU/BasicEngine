#include "RubiksCube.h"
#include "../Engine3D/scene.h"
#include <iostream>

using namespace std;

RubiksCube::RubiksCube(int size) : Game()
{
    cube_size = size;

}

RubiksCube::RubiksCube(float angle ,float relationWH, float near1, float far1, int size) : RubiksCube(angle,relationWH,near1,far1)
{ 	
    cube_size = size;
}

RubiksCube::~RubiksCube(void)
{
}


RubiksCube::initialize_walls()
{
    if (shapes.size() != glm::pow(cube_size, 3)){
        cout << 'Could not initialize shapes' << endl;
        return;
    }
    front_to_back_walls = 
    
}
