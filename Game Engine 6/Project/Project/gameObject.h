#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "gtx\transform.hpp"

#include <string>

#include "Mesh.h"
#include "boundingBox.h"

class gameObject
{
protected:
	mesh *Mesh; //the mesh that is used for the object
	std::string textureName; //the name of the texture for the object
	std::string shaderName; //the name of the shader used to render the object
	glm::vec3 position; //the position of the object within thw world
	glm::vec3 color; //the color of the object
	glm::mat4 transform; //the transformation matrix for the object
	GLuint vboHandle[3]; //the three vbo IDs for the position and UV vbo handle and normal handle
	GLuint vaoHandle; //the ID for the vao
	
	float velocity;

	glm::vec3 vel;

	void setMinMax(); //set the min and max bounding box points

	

public:
	gameObject(); //create an object within the game
	gameObject(mesh Mesh, glm::vec3 color, std::string shaderName, glm::vec3 Position); //create an object with a mesh, color, shader and position
	gameObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position); //create an object with a mesh, texture, shader and position
	void render(); //the render fucntion for the object
	void createVAO(mesh Mesh); //the function used to create the vbo and vao

	std::string getShaderName(); //return the name of the objects shader
	std::string getTextureName(); //return the name of the objects texture
	glm::mat4 getTransformMatrix(); //return the transform matrix for the object
	void scale(glm::vec3 scale); //scale the object

	glm::vec3 getPosition();
	glm::vec3 getVel();
	void setPosition(glm::vec3 position);
	void setVel(glm::vec3 velocity);

	bool chase(gameObject player);

	boundingBox bb;

};
#endif
