#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "gameObject.h"
#include "camera.h"

class player : public gameObject
{
private:
	
	bool canJump;
public:
	player();
	~player();
	void setUpPlayerObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position);
	void setUpPlayerCamera(float FOV, float nearPlane, float farPlane, float aspect, glm::vec3 position, glm::vec3 target);
	camera playerCam; // the camera that follows the player
	void setCanJump(bool canJump);
	bool getCanJump();
	int health, score;
};
#endif