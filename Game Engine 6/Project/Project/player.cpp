#include "stdafx.h"
#include "player.h"

player::player()
{
	canJump = true;
}

player::~player()
{

}

void player::setUpPlayerCamera(float FOV, float nearPlane, float farPlane, float aspect, glm::vec3 position, glm::vec3 target)
{
	playerCam.setFOV(FOV); //sets the camera FOV
	playerCam.setNearPlane(nearPlane); //sets the near clipping plane
	playerCam.setFarPlane(farPlane); //sets the far clipping plane
	playerCam.setAspect(aspect); //sets the aspect ratio for the camera
	playerCam.setPosition(position); //set the position
	playerCam.setTarget(target); //sets the target
	playerCam.update();
	playerCam.setProjection(FOV,aspect,nearPlane,farPlane); //sets the projection matrix for the camera
}

void player::setUpPlayerObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position)
{
	this->Mesh = &Mesh; //set the mesh for the object to the mesh
	this->textureName = textureName; //set the name of the objects texture
	this->shaderName = shaderName; //set the name of the objects shader
	this->position = Position; //set the current position of the object within the world
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(position)); //set up the transform matrix
	createVAO(Mesh); //create the vao for the object when it is created
	setMinMax();
	this->health = 20; //20 for testing
	this->score = 0;
}

void player::setCanJump(bool canJump)
{
	this->canJump = canJump;
}

bool player::getCanJump()
{
	return canJump;
}

void player::reset(glm::vec3 Position){
	this->health = 100;
	this->score = 0;
	this->position = Position; //set the current position of the object within the world
}