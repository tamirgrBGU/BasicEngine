#include "MovableGLM.h"
#include <stdio.h>
#include "../res/includes/glm/gtc/matrix_transform.hpp"
#include "../res/includes/glm/gtc/quaternion.hpp"


static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

MovableGLM::MovableGLM()
{
	ZeroTrans();
}

glm::mat4 MovableGLM::MakeTrans(const glm::mat4 &prevTransformations) const
{
	return prevTransformations * MakeTrans();
}

glm::mat4 MovableGLM::MakeTrans() const
{
	return   rot * trans * scl  ;
}

void MovableGLM::MyTranslate(glm::vec3 delta,int mode)
{
	trans = glm::translate(trans,delta);
}

void  MovableGLM::MyRotate(float angle,const glm::vec3 &vec,int mode)
{
	rot = glm::rotate(rot,angle,vec);
}

glm::mat3 MovableGLM::GetTransposedRot() {
	glm::mat3 transposed_rot = glm::mat3(
		glm::vec3(rot[0][0], rot[0][1], rot[0][2]),
		glm::vec3(rot[1][0], rot[1][1], rot[1][2]),
		glm::vec3(rot[2][0], rot[2][1], rot[2][2])
    );
	return transposed_rot;
}

void MovableGLM::RotateRelative(float angle, const glm::vec3& vec) {
	MyRotate(angle, vec * GetTransposedRot(), 0);
}
	
void  MovableGLM::MyScale(glm::vec3 scale)
{
	scl = glm::scale(scl,scale);
}

void MovableGLM::ZeroTrans()
{
	trans = glm::mat4(1);
	rot = glm::mat4(1);
	scl = glm::mat4(1);
}