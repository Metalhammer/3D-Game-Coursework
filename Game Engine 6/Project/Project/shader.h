#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <string>

class shader
{
private:
	GLuint programID; // the id for the program

public:
	shader();
	shader(GLuint id); //create the shader
	void useProgram(); //ready the shader for use when rendering
	void disableProgram(); //disable the shader
	GLuint getProgramID(); //return the shaders program ID

};
#endif