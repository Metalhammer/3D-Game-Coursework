#ifndef PICKUPS_H
#define PICKUPS_H

#include "stdafx.h"
#include "gameObject.h"
#include "camera.h"

class pickups : public gameObject
{
private:
	bool canFall;
public:
	pickups();
	~pickups();
	void setUpPickupObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position);
	void setCanFall(bool canFall);
	bool getCanFall();
};
#endif