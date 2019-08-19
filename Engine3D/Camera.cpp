#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
	
Camera::Camera(glm::vec3& pos,const glm::vec3& forward, float fov, float zNear, float zFar)
	{
		this->projection =  glm::perspective(fov,1.0f , zNear, zFar);
		this->projection = this->projection;
		this->fov = fov;
		this->near = zNear;
		this->far = zFar;
}

	void Camera::SetProjection(float zNear, float zFar)
	{
	
		this->projection = glm::perspective(fov,1.0f , zNear, zFar);
		this->near = zNear;
		this->far = zFar;		
	}

