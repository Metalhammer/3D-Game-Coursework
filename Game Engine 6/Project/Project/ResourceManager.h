//=====================================================================================
//MANAGES THE LOADING AND HANDLING OF RESOURCES, INCLUDEING TEXTURES, MESHES AND SHADERS
//=====================================================================================

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

#include "Mesh.h"
#include "texture.h"
#include "shader.h"

class resourceManager
{
private:
	std::map<std::string, mesh> meshes; // a map containing all of the loaded meshes
	std::map<std::string, texture> textures; // a map of all the textures
	std::map<std::string, shader> shaders; //a map of all the shaders

public:
	resourceManager(); //create a resoure manager

	void loadMesh(std::string path, std::string meshName); //load a mesh from an obj file and give it a name
	void loadTexture(std::string textureName); //load a texture
	void loadShader(std::string frag, std::string vert, std::string name); //load a pair of shaders

	mesh& getMesh(std::string meshName); // return a mesh stored in the mesh map
	shader& getShader(std::string shaderName); //return the shader stored in the shader map
	texture& getTexture(std::string textureName); //return the texture stored in the texture map
};
#endif