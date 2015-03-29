#include "stdafx.h"
#include "shader.h"

shader::shader()
{
}

shader::shader(GLuint id)
{
	programID = id; //set the program id
}

void shader::useProgram()
{
	gl::UseProgram(programID); //set the shader ready for use
}

void shader::disableProgram()
{
	gl::UseProgram(NULL); //use no shader when rendering
}

GLuint shader::getProgramID()
{
	return programID; //returns the shaders program id
}