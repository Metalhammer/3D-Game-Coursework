#include "stdafx.h"
#include "moveable.h"

moveable::moveable(){
	REFRESH_RATE = 0.01666666666f;
}

moveable::~moveable(){
}

void moveable::euler(camera *cam){
	glm::vec3 pos = cam->getPosition();
	glm::vec3 vel = cam->getVelocity();
	float accel = -65;



	float newVY = vel.y + (accel*REFRESH_RATE);
	float newRY = pos.y + ((vel.y + newVY)*(REFRESH_RATE/2));

	vel.y = newVY;
	pos.y = newRY;

	if (pos.y <= 0){
	//	vel.y = 0;
	//	pos.y = 0;
	//	cam->JUMPING_UP = false;
	//	cam->WALKING = true;
	}

	cam->setVelocity(vel);
	cam->setPosition(pos);

}