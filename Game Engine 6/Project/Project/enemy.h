#ifndef ENEMY_H
#define ENEMY_H

#include "stdafx.h"
#include "gameObject.h"
#include "camera.h"

class enemy : public gameObject
{
private:
	
public:
	enemy();
	~enemy();
	void setUpEnemyObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position);

};
#endif