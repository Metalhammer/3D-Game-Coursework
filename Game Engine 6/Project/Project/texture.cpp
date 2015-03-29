#include "stdafx.h"
#include "texture.h"

texture::texture()
{};

texture::texture(GLuint id, int width, int height)
{
	textureID = id; //set the texture id
	this->width = width; //set the width of the texture
	this->height = height; //set the height of the texture
}

void texture::useTexture()
{
	gl::BindTexture(gl::TEXTURE_2D, textureID); //bind the texture for drawing
}