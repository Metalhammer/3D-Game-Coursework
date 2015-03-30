#include "stdafx.h"
#include "pickups.h"

pickups::pickups()
{

}

pickups::~pickups()
{

}

void pickups::setUpPickupObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position)
{

	
	//this->vel(0,1,0);
	//this->velocity = 0.00075f;
	this->Mesh = &Mesh; //set the mesh for the object to the mesh
	this->textureName = textureName; //set the name of the objects texture
	this->shaderName = shaderName; //set the name of the objects shader
	this->position = Position; //set the current position of the object within the world
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(position)); //set up the transform matrix
	createVAO(Mesh); //create the vao for the object when it is created
	setMinMax();
}

void pickups::setCanFall(bool canFall)
{
	this->canFall = canFall;
}

bool pickups::getCanFall()
{
	return canFall;
}