#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

	
	Camera::Camera(float fov, float zNear, float zFar)
	{
		if(fov > 0) //prerspective
		{
			this->projection =  glm::perspective(fov,1.0f , zNear, zFar);
		}
		else  //ortho
		{
			this->projection = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,zNear,zFar);
		}
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

