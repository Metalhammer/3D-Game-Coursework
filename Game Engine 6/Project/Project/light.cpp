#include "stdafx.h"
#include "light.h"

light::light(){}

light::light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
	this->position = position;
	this->ambientIntensity = ambient;
	this->diffuseIntensity = diffuse;
	this->specularIntensity = specular;
}

glm::vec3 light::getPosition(){
	return position;
}

glm::vec3 light::getAmbientIntensity(){
	return ambientIntensity;
}

glm::vec3 light::getDiffuseIntensity(){
	return diffuseIntensity;
}

glm::vec3 light::getSpecularIntensity(){
	return specularIntensity;
}