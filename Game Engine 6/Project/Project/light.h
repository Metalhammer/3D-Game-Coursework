#ifndef LIGHT_H
#define LIGHT_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>

class light
{
private:
	glm::vec3 position; //the position of the light
	glm::vec3 ambientIntensity; //the intensity of the lights ambient component
	glm::vec3 diffuseIntensity; //the intensity of the lights diffuse component
	glm::vec3 specularIntensity; // the intensity of the lights specular component

public:
	light();
	light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	glm::vec3 getPosition(); //get the lights position
	glm::vec3 getAmbientIntensity(); //get the ambient intensity of the light
	glm::vec3 getDiffuseIntensity(); //get the diffuse intensity of the light
	glm::vec3 getSpecularIntensity(); //get the specular intensity of the light

};
#endif