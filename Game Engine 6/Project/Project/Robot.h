#ifndef __ROBOT_H
#define __ROBOT_H

#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>

// constants for arm and leg movement states
const char BACKWARD_STATE = 0;
const char FORWARD_STATE  = 1;

// index constants for accessing arm and leg array data
const char LEFT  = 0;
const char RIGHT = 1;

class Robot
{
private:
	char legStates[2];	
	char armStates[2];

	float legAngles[2];
	float armAngles[2];

	// methods to draw the parts of the robot
	void DrawArm(float xPos, float yPos, float zPos);
	void DrawHead(float xPos, float yPos, float zPos);
	void DrawTorso(float xPos, float yPos, float zPos);
	void DrawLeg(float xPos, float yPos, float zPos);
	void DrawFoot(float xPos, float yPos, float zPos);

public:

	Robot();
	virtual ~Robot();

	// draws the entire robot
	void DrawRobot(float xPos, float yPos, float zPos);

	// updates the robot data
	void Prepare(float dt);
};

#endif