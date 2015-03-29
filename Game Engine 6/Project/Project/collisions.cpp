#include "stdafx.h"
#include "collisions.h"

collisions::collisions()
{

}

collisions::~collisions()
{

}

bool collisions::checkCollision(gameObject box1, gameObject box2)
{
	glm::vec3 box1Position = box1.getPosition();
	glm::vec3 box2Position = box2.getPosition();

	glm::vec3 box1Min  = box1Position + box1.bb.getMin();
	glm::vec3 box1Max = box1Position + box1.bb.getMax();
	glm::vec3 box2Min = box2Position + box2.bb.getMin();
	glm::vec3 box2Max = box2Position + box2.bb.getMax();

	if(box1Min.x < box2Max.x && box1Max.x > box2Min.x && box1Min.y < box2Max.y && box1Max.y > box2Min.y && box1Min.z < box2Max.z && box1Max.z > box2Min.z)
	{
		return true;
	}
	else return false;
}