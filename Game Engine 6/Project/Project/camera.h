#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "gtx\transform.hpp"

#include <iostream>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>

class camera
{
private:
	glm::vec3 position; //the position of the camera
	glm::vec3 velocity; 
	glm::vec3 target; //the position in the world the camera is directed at
	glm::vec3 up; //y axis relative to the camera
	glm::vec3 right; //vector relative to the right of the camera
	glm::vec3 forward; //the vector relative to the front of the camera
	glm::vec3 cameraFront;
	float pitch; //the rotation of the camera in the x axix
	float yaw; // the rotation of the camera in the y axis
	float orientation; //the orientation of the camera around the y axis

	glm::mat4 view; //the view matrix for the camera
	glm::mat4 projection; //the projection matrix for the camera

	float FOV; //the field of view for the camera
	float nearPlane; //the near clipping plane for the camera
	float farPlane; //the far clipping plane for the camera
	float aspect; //the aspect ratio for the camera

	
public:
	camera();
	camera(float FOV, float nearPlane, float farPlane, float aspect); //create a camera with fov, clipping planes and aspect ratio
	camera(float FOV, float nearPlane, float farPlane, float aspect, glm::vec3 position, glm::vec3 target);//create a camera with fov, clipping planes, aspect ratio, position and target
	void setPosition(glm::vec3 position); //sets the position of the camera
	void setVelocity(glm::vec3 velocity);
	void setTarget(glm::vec3 target); //sets the target of the camera
	void update(); //update the position, target and up vector

	glm::mat4 getViewMatrix(); //return the view matrix
	glm::mat4 getProjectionMatrix(); //return the projection matrix

	void look(float x, float y, float sensitivity); //look around with the camera
	void pan(float x, float y); //pan the camera
	void rotate(float x, float y); //rotate the camera around the target
	void zoom(float z); //zoom the camera into the target
	void forwards(float speedMod); //move the camera forwads
	void backwards(float speedMod); //move the camera backwards
	void strafeLeft(float speedMod); //move the camera left
	void strafeRight(float speedMod); //move the camera right

	bool canJump;

	glm::vec3 getPosition(); //return the position of the camera
	glm::vec3 getVelocity();
	glm::vec3 getForward();

	void setFOV(float FOV);
	void setNearPlane(float plane);
	void setFarPlane(float plane);
	void setAspect(float aspect);
	void setProjection(float FOV, float aspect, float nearPlane, float farPlane);

	
};
#endif