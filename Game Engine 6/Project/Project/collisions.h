#ifndef COLLISIONS_H
#define COLLISIONS_H


#include "stdafx.h"
#include <glm.hpp>
#include "boundingBox.h"
#include "gameObject.h"

class collisions
{
private:

public:
	collisions();
	~collisions();

	bool checkCollision(gameObject box1, gameObject box2);

protected:

};
#endif // !COLLISIONS_H
