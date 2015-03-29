#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Mesh.h"

mesh::mesh()
{
}

//=====================================================
// GETTERS
//=====================================================

std::vector<glm::vec3> mesh::getVertices()
{
	return vertices;
}

std::vector<glm::vec3> mesh::getNormal()
{
	return normals;
}

std::vector<glm::vec2> mesh::getUV()
{
	return  uvs;
}

std::vector<GLuint> mesh::getIndex()
{
	return indices;
}

//=====================================================
// SETTERS
//=====================================================

void mesh::addVertex(glm::vec3 Vertex)
{
	vertices.push_back(Vertex);
}

void mesh::addNormal(glm::vec3 Normal)
{
	normals.push_back(Normal);
}

void mesh::addUV(glm::vec2 UV)
{
	uvs.push_back(UV);
}

void mesh::addIndex(GLuint index)
{
	indices.push_back(index);
}

void mesh::addTexName(std::string texName)
{
	textureName = texName;
}