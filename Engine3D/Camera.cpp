#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
	
Camera::Camera(glm::vec3& pos,const glm::vec3& forward, float fov, float zNear, float zFar)
	{
		this->projection =  glm::perspective(fov,1.0f , zNear, zFar);
		this->projection = this->projection;
		this->fov = fov;
		this->near = zNear;
		this->far = zFar;
		MyRotate(90.0f,glm::vec3(0,1,0),-1);
		//MyTranslate(pos,1);
}

	void Camera::SetProjection(float zNear, float zFar)
	{
	
		this->projection = glm::perspective(fov,1.0f , zNear, zFar);
		this->near = zNear;
		this->far = zFar;		
	}

	void Camera::MoveForward(float amt)
	{
		MyTranslate(glm::vec3(0,0,amt),1);
	}
	void Camera::MoveRight(float amt)
	{
		MyTranslate(glm::vec3(amt,0,0),1);
	}
	void Camera::MoveUp(float amt)
	{
		MyTranslate(glm::vec3(0,amt,0),1);
	}
	void Camera::Pitch(float angle)
	{
		MyRotate(angle,glm::vec3(0,0,1),-1);
	}
	void Camera::RotateY(float angle)
	{
		MyRotate(angle,glm::vec3(0,1,0),-1);
	}