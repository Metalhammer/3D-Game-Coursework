#include "stdafx.h"
#include "boundingBox.h"

boundingBox::boundingBox()
{

}

boundingBox::~boundingBox()
{

}

void boundingBox::setMax(glm::vec3 max)
{
	maxPoint = max;
}

void boundingBox::setMin(glm::vec3 min)
{
	minPoint = min;
}

glm::vec3 boundingBox::getMax()
{
	return maxPoint;
}

glm::vec3 boundingBox::getMin()
{
	return minPoint;
}
