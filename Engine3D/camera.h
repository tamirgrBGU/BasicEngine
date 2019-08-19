#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H
//#include <glm/glm.hpp>
#include "MovableGLM.h"

class Camera : public MovableGLM
{
public:
	Camera(glm::vec3& pos,const glm::vec3& forward, float fov, float zNear, float zFar);

	inline glm::mat4 GetViewProjection() const	{
		return projection;}	

	inline float GetAngle(){
		return fov;}

	inline float GetNear(){
		return near;}

	inline float GetFar(){
		return far;}
	
	void SetProjection(float zNear, float zFar);
protected:
private:
	glm::mat4 projection;
	float fov;
	float far,near;
};

#endif
