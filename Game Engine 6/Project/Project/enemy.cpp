#include "stdafx.h"
#include "enemy.h"

enemy::enemy()
{

}

enemy::~enemy()
{

}

void enemy::setUpEnemyObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position)
{
	//this->velocity = 0.075f;
	this->velocity = 0.0080f;
	this->Mesh = &Mesh; //set the mesh for the object to the mesh
	this->textureName = textureName; //set the name of the objects texture
	this->shaderName = shaderName; //set the name of the objects shader
	this->position = Position; //set the current position of the object within the world
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(position)); //set up the transform matrix
	createVAO(Mesh); //create the vao for the object when it is created
	setMinMax();
}

