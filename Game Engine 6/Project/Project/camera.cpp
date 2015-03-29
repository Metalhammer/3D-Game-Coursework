#include "stdafx.h"
#include "camera.h"

camera::camera()
{

}

camera::camera(float FOV, float nearPlane, float farPlane, float aspect)
{
	this->FOV = FOV; //sets the camera FOV
	this->nearPlane = nearPlane; //sets the near clipping plane
	this->farPlane = farPlane; //sets the far clipping plane
	this->aspect = aspect; //sets the aspect ratio for the camera
	projection = glm::perspective(FOV,aspect,nearPlane,farPlane); //sets the projection matrix for the camera
	canJump = true;
}

camera::camera(float FOV, float nearPlane, float farPlane, float aspect, glm::vec3 position, glm::vec3 target)
{
	this->FOV = FOV; //sets the camera FOV
	this->nearPlane = nearPlane; //sets the near clipping plane
	this->farPlane = farPlane; //sets the far clipping plane
	this->aspect = aspect; //sets the aspect ratio for the camera
	setPosition(position); //set the position
	setTarget(target); //sets the target
	update();
	projection = glm::perspective(FOV,aspect,nearPlane,farPlane); //sets the projection matrix for the camera
	canJump = true;
}

void camera::setPosition(glm::vec3 position)
{
	this->position = position; //set the position of the camera
}

void camera::setVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}

void camera::setTarget(glm::vec3 target)
{
	this->target = target; //sets the cameras target
}

void camera::update()
{
	forward = glm::normalize(-(position-cameraFront-position)); //find the forward vector
	right = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f),forward)); //find the right vector
	up = glm::normalize(glm::cross(forward,right)); //set the up vector
	view = glm::lookAt(position,position+cameraFront,up); //sets the view matrix for the camera
}

glm::mat4 camera::getProjectionMatrix()
{
	return projection; //return the projection matrix
}

glm::mat4 camera::getViewMatrix()
{
	return view; //return the view matrix
}

void camera::look(float x, float y, float sensitivity)
{
	float xOffset = x*sensitivity;
	float yOffset = y*sensitivity;

	yaw-=xOffset;
	pitch+=yOffset;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	//glm::normalize(target);
	//target = target + right*x; //set the target of the camera to its current position + the right vector multiplied by the mouse X offset (look left and right)
	//target = target + up*y; //set the target of the camera to its current position + the up vector multiplied by the mouse Y offset (look up and down)
	
	//std::cout << x << " " << y << " " << target.x << " " << target.y << " " << target.z << std::endl;
	update(); //update the camera values
}

void camera::forwards(float speedMod)
{
	forward = glm::vec3(forward.x,0.0,forward.z); //keeps the camera from moving along the y axis
	position += forward*speedMod; //move the camera along the forward vector by a small amount
	//target += forward*speedMod; //move the target along the forward vector by a small amount
	update(); //update the camera values
}

void camera::backwards(float speedMod)
{
	forward = glm::vec3(forward.x,0.0,forward.z);
	position -= forward*speedMod; //move the camera back on the forward vector
	//target -= forward*speedMod; //move the target back along the forard vector
	update(); //update the camera values
}

void camera::strafeLeft(float speedMod)
{
	right = glm::vec3(right.x,0.0,right.z);
	position += right*speedMod; //move the camera along the right vector
	//target += right*speedMod; //move the target along the right vector
	update(); //update the camera values

	
}

void camera::strafeRight(float speedMod)
{
	right = glm::vec3(right.x,0.0,right.z);
	position -= right*speedMod;//move the camera along the right vector
	//target -= right*speedMod;	//move the target along the right vector
	update(); //update the camera values

}

glm::vec3 camera::getPosition()
{
	return position;
}

glm::vec3 camera::getVelocity()
{
	return velocity;
}

glm::vec3 camera::getForward()
{
	return forward;
}

void camera::rotate(float x, float y)
{
	pitch += y;
	yaw += x;

	if ((pitch>360) || (pitch<-360)) pitch=0;
	if ((yaw>360) || (yaw<-360)) yaw=0;

	glm::mat4 trans = glm::translate(glm::mat4(1), -target);
	glm::mat4 invtrans = glm::translate(glm::mat4(1), target);
	glm::mat4 yRotate = glm::rotate(glm::mat4(1), x, glm::vec3(0,1,0));

	position = glm::vec3(invtrans * yRotate * trans * glm::vec4(position,1));
	position = glm::rotate(position,-y,right);
	update();
}

void camera::zoom(float z)
{
	float speed  = 0.02;
	forward = target - position;
	position = position + forward*z*speed;
	update();
}

void camera::pan(float x, float y)
{
	position = position + right*x;
	target = target + right*x;
	position = position + up*y;
	target = target + up*y;
	update();
}

void camera::setFOV(float fov)
{
	this->FOV = fov;
}

void camera::setNearPlane(float plane)
{
	this->nearPlane = plane;
}

void camera::setFarPlane(float plane)
{
	this->farPlane = plane;
}

void camera::setAspect(float aspect)
{
	this->aspect = aspect;
}

void camera::setProjection(float FOV, float aspect, float nearPlane, float farPlane)
{
	this->projection = glm::perspective(FOV,aspect,nearPlane,farPlane);
}