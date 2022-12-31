#pragma once
#include "../res/includes/glm/glm.hpp"

class MovableGLM 
{
public:
	MovableGLM();
	glm::mat4 MakeTrans(const glm::mat4 &prevTransformations) const;
	glm::mat4 MakeTrans() const;
	void MyTranslate(glm::vec3 delta, int mode);
	void MyRotate(float angle, const glm::vec3 &vec, int mode);
	void MyScale(glm::vec3 scale);
	void ZeroTrans();
	void RotateRelative(float angle, const glm::vec3& vec, MovableGLM other);
	glm::mat3 GetTransposedRot() {
    	glm::mat3 cube_rotation = glm::mat3(
            glm::vec3(rot[0].x, rot[0].y, rot[0].z),
            glm::vec3(rot[1].x, rot[1].y, rot[1].z),
            glm::vec3(rot[2].x, rot[2].y, rot[2].z)
    );
	return cube_rotation;}
private:
	glm::mat4 trans;
	glm::mat4 rot;
	glm::mat4 scl;
};