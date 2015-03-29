//=====================================================================================
//A SET OF VERTICES USED TO CREATE A GAME OBJECT, CANNOT BE RENDERED ON ITS OWN
//=====================================================================================

#ifndef MESH_H
#define MESH_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <iostream>
#include <vector>

class mesh
{
private:
	std::vector<glm::vec3> vertices; // a vector of all the vertices in the mesh
	std::vector<glm::vec2> uvs; // a vector of all the uv coordinates in the mesh
	std::vector<glm::vec3> normals; // a vector of all the normals in the mesh
	std::vector<GLuint> indices; // a vector of indices in the mesh
	std::string textureName; // name of the meshes texture

public:
	mesh(); //create the mesh

	std::vector<glm::vec3> getVertices(); //return the vector of vertices
	std::vector<glm::vec3> getNormal(); //return the vector of normals
	std::vector<glm::vec2> getUV(); //return the vector of UVs
	std::vector<GLuint> getIndex(); //return the vector of indices

	void addVertex(glm::vec3 Vertex); //add a vertex to the vector of vertices
	void addNormal(glm::vec3 Normal); //add a normal to the vector of normals
	void addUV(glm::vec2 UV); //add a UV to the vector of UVs
	void addIndex(GLuint); //add an index to the vctor of indices
	void addTexName(std::string texName); //set the name of the texture of the mesh


};
#endif