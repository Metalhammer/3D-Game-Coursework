//=====================================================================================
//THE TEXTURE AND ALL ITS VALUES
//=====================================================================================

#ifndef TEXTURE_H
#define TEXTURE_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>

class texture
{
private:
	GLuint textureID; //the id for the texture
	int width; //the width of the texture
	int height; //the height of the texture

public:
	texture();
	texture(GLuint id, int width, int height); //create the texture
	void useTexture(); //bind the texture ready for rendering

};
#endif