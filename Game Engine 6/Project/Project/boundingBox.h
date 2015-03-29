#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


#include "stdafx.h"
#include <glm.hpp>

class boundingBox
{
private:
	glm::vec3 minPoint; //lowest point bounding box
	glm::vec3 maxPoint; //maximum point bounding box

public:
	boundingBox();
	~boundingBox();

	void setMin(glm::vec3 min);
	void setMax(glm::vec3 max);

	glm::vec3 getMin();
	glm::vec3 getMax();




protected:

};
#endif // !BOUNDINGBOX_H