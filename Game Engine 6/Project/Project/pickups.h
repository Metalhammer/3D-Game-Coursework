#ifndef PICKUPS_H
#define PICKUPS_H

#include "stdafx.h"
#include "gameObject.h"
#include "camera.h"

class pickups : public gameObject
{
private:
	
public:
	pickups();
	~pickups();
	void setUpPickupObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position);

};
#endif