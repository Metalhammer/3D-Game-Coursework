#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "stdafx.h"
#include "camera.h"
#include <glm.hpp>

class moveable{

private:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 accel;

	float REFRESH_RATE;

public:
	moveable();
	~moveable();
	void euler(camera *cam);

	void setPos(glm::vec3 pos);
	void setVel(glm::vec3 vel);
	void setAccel(glm::vec3 accel);

	glm::vec3 getPos();
	glm::vec3 getVel();
	glm::vec3 getAccel();
};

#endif 