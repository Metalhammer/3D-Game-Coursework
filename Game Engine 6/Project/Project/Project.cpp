#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <cstdlib>

#include <iostream>
#include <string>

#include "ResourceManager.h"
#include "Mesh.h"
#include "gameObject.h"
#include "camera.h"
#include "Robot.h"
#include "collisions.h"
#include "moveable.h"
#include "player.h"
#include "enemy.h"
#include "pickups.h"
//sound
#include <irrKlang.h>
using namespace irrklang;

float gamemode;
resourceManager RM;
GLFWwindow* window; // the render window
glm::mat4 modelMatrix(1.0); //the model matrix used for rendering
std::vector<gameObject> objects; //vector of objects in the game
std::vector<pickups> pickupObject;
std::vector<pickups> bonusObject;
std::vector<enemy> enemies;
std::vector<gameObject> bullets;
bool loaded = false; //is the engine loaded
bool canShoot = true;
collisions coll;
moveable move;
player player1;
float spawnTimer = 0;
float startTime = 0;
int enemiesSpawned = 1;
float lastShot = 0;
int highscore = 0;

float a, b, x, z;
float aDiff, bDiff, xDif, zDif;
float dist = 0;
float dist2 = 0;
float pickupTimer = 0;
float bonusTimer = 0;
int pickupsSpawned = 1;
float timer2 = 10;
float timer3 = 0;
int bonusSpawned = 1;
int killCounter = 0;
//the current and old positions of the mouse
double posX,posY,oldX,oldY;

ISoundEngine *SoundEngine = createIrrKlangDevice();


void input()
{
	glfwGetCursorPos(window,&posX,&posY); //get the position of the cursor within the window
	//get the difference between the old and current mouse position
	float moveX = (float)(oldX - posX); 
	float moveY = (float)(oldY - posY);

	if(gamemode == 1) //if the game is playing, get input
	{
		//look around with the camera
		player1.playerCam.look(moveX,moveY, 0.05f);

		//Shoot
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) && glfwGetTime()-1 > lastShot){
			lastShot = glfwGetTime();
			gameObject newBullet(RM.getMesh("block"), "red.jpg", "shader", player1.getPosition());
			newBullet.scale(glm::vec3(1,1,1));
			newBullet.setVel(player1.playerCam.getForward());
			bullets.push_back(newBullet);
			SoundEngine->play2D("../Resources/playerShoot.wav", gl::FALSE_);
			canShoot = false;
		}

		//move forwrds
		if (glfwGetKey(window,GLFW_KEY_W)){
		player1.playerCam.forwards(0.05);} //move forward when W is pressed

		if (glfwGetKey(window,GLFW_KEY_W) && glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)){
				player1.playerCam.forwards(0.1);} //move forward when W is pressed

		//move backwards
		if (glfwGetKey(window,GLFW_KEY_S)){
				player1.playerCam.backwards(0.05);} //move backward when S is pressed

		//move left
		if (glfwGetKey(window,GLFW_KEY_A)){
				player1.playerCam.strafeLeft(0.05);} //move left when A is pressed

		//move right
		if (glfwGetKey(window,GLFW_KEY_D)){
				player1.playerCam.strafeRight(0.05);} //move right when D is pressed


		if (glfwGetKey(window,GLFW_KEY_SPACE)){
			if (player1.getCanJump() == true){
				glm::vec3 vel = player1.playerCam.getVelocity();
				vel.y = 20;
				player1.playerCam.setVelocity(vel);
				player1.setCanJump(false);

			}
		}

	//go to the menu
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)){
		gamemode = 3;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
	}
	}

	if(gamemode == 3) //game mode 3 is the menu
	{
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)){
		double xpos, ypos; 
		glfwGetCursorPos(window,&xpos,&ypos);//get the mouse input

		if(xpos >379 && xpos < 853 && ypos > 82 && ypos <268)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
			startTime = glfwGetTime();
			gamemode = 1;
			enemiesSpawned = 0;
		}

		if(xpos >379 && xpos < 853 && ypos > 369 && ypos <553)
		{
			//destroy window
			glfwDestroyWindow(window);
			//terminate glfw
			glfwTerminate();
			exit(0);
		}
		}
	}

	//make the old position of the mouse the current position
	oldX = posX;
	oldY = posY;
}

void render()
{
	//clear depth buffer before rendering
	gl::Clear(gl::DEPTH_BUFFER_BIT);
	//clear color buffer before rendering
	gl::Clear(gl::COLOR_BUFFER_BIT);

	//==================================
	// lighting and render for skybox
	//==================================
	RM.getShader(objects[1].getShaderName()).useProgram(); //ready the shader for rendering

	GLuint modelMatrixID = gl::GetUniformLocation(RM.getShader(objects[1].getShaderName()).getProgramID(), "mModel");
	GLuint viewMatrixID = gl::GetUniformLocation(RM.getShader(objects[1].getShaderName()).getProgramID(), "mView");
	GLuint projectionMatrixID = gl::GetUniformLocation(RM.getShader(objects[1].getShaderName()).getProgramID(), "mProjection");

	gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, &player1.playerCam.getViewMatrix()[0][0]);
	gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, &player1.playerCam.getProjectionMatrix()[0][0]);

	gl::Disable(gl::DEPTH_TEST);
	RM.getTexture(objects[1].getTextureName()).useTexture(); //bind the texture for rendering
	modelMatrix = objects[1].getTransformMatrix(); //set the model matrix for rendering
	gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &modelMatrix[0][0]);
	objects[1].render(); //render the object
	gl::Enable(gl::DEPTH_TEST);

	//======================================
	// lighting and render for all objects
	//======================================

	//check if the activeate spotlight
	GLuint activeShaderID;
	glm::vec3 lightpos;
	glm::vec3 ambientLight;

	if(player1.score < 1000 ){
		RM.getShader(objects[2].getShaderName()).useProgram(); //ready the shader for rendering
		activeShaderID = RM.getShader(objects[2].getShaderName()).getProgramID();
		lightpos = glm::vec3(0,10,0);
		ambientLight = glm::vec3(0.5,0.5,0.5);
	}
	else{
		float lightHeight;
		RM.getShader("spotShader").useProgram(); //ready the shader for rendering
		activeShaderID = RM.getShader("spotShader").getProgramID();
		if (11-player1.score/500 < 1) lightHeight = 1;
		else lightHeight = 11-player1.score/500;
		lightpos = glm::vec3(0,lightHeight,0);
		ambientLight = glm::vec3(0.2,0.2,0.2);
	}

	//set up light for render

	//the position of the light
	GLuint light = gl::GetUniformLocation(activeShaderID, "lightPosition");
	gl::Uniform3f(light,lightpos.x,lightpos.y,lightpos.z);

	GLuint lightTarget = gl::GetUniformLocation(activeShaderID, "lightTarget");
	gl::Uniform3f(lightTarget,0,0,0);

	GLuint innerConeAngle = gl::GetUniformLocation(activeShaderID, "innerConeAngle");
	gl::Uniform1f(innerConeAngle, glm::cos(glm::radians(1.0)));

	GLuint outerConeAngle = gl::GetUniformLocation(activeShaderID, "outerConeAngle");
	gl::Uniform1f(outerConeAngle, glm::cos(glm::radians(3.0)));

	//the reflectivity of the objects
	GLuint AmbientReflectivity = gl::GetUniformLocation(activeShaderID, "AmbientReflectivity");
	gl::Uniform3f(AmbientReflectivity,1.0,1.0,1.0);

	GLuint DiffuseReflectivity = gl::GetUniformLocation(activeShaderID, "DiffuseReflectivity");
	gl::Uniform3f(DiffuseReflectivity,1.0,1.0,1.0);

	GLuint SpecularReflectivity = gl::GetUniformLocation(activeShaderID, "SpecularReflectivity");
	gl::Uniform3f(SpecularReflectivity,3.0,3.0,3.0);

	//the intensity of the light
	GLuint AmbientIntensity = gl::GetUniformLocation(activeShaderID, "AmbientIntensity");
	gl::Uniform3f(AmbientIntensity,ambientLight.x,ambientLight.y,ambientLight.z);

	GLuint DiffuseIntensity = gl::GetUniformLocation(activeShaderID, "DiffuseIntensity");
	gl::Uniform3f(DiffuseIntensity,1.0,1.0,1.0);

	GLuint SpecularIntensity = gl::GetUniformLocation(activeShaderID, "SpecularIntensity");
	gl::Uniform3f(SpecularIntensity,1.0,1.0,1.0);

	GLuint SpecularExponent = gl::GetUniformLocation(activeShaderID, "SpecularExponent");
	gl::Uniform1f(SpecularExponent,64);

	//set up matrices for render

	GLuint normalMatrixID = gl::GetUniformLocation(activeShaderID, "mNormal");
	modelMatrixID = gl::GetUniformLocation(activeShaderID, "mModel");
	viewMatrixID = gl::GetUniformLocation(activeShaderID, "mView");
	projectionMatrixID = gl::GetUniformLocation(activeShaderID, "mProjection");

	glm::mat4 mv = player1.playerCam.getViewMatrix() * modelMatrix;
	glm::mat3 normalMatrix = glm::mat3( glm::vec3(mv[0]) , glm::vec3(mv[1]), glm::vec3(mv[2]));
	gl::UniformMatrix3fv(normalMatrixID, 1, gl::FALSE_, &normalMatrix[0][0]);
	gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, &player1.playerCam.getViewMatrix()[0][0]);
	gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, &player1.playerCam.getProjectionMatrix()[0][0]);

	//render objects

		for(int i=2; i<objects.size();i++)
		{
			RM.getTexture(objects[i].getTextureName()).useTexture(); //bind the texture for rendering
			
			modelMatrix = objects[i].getTransformMatrix(); //set the model matrix for rendering

			//set up the model, view and porjection matrix for use with the shader

			gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &modelMatrix[0][0]);
			

			//================================================
			//RENDER
			//================================================

			objects[i].render(); //render the object
			
		}

		for (int i=0; i<enemies.size();i++)
		{
			RM.getTexture(enemies[i].getTextureName()).useTexture(); //bind the texture for rendering
			modelMatrix = enemies[i].getTransformMatrix(); //set the model matrix for rendering

			gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &modelMatrix[0][0]);

			//================================================
			//RENDER
			//================================================

			enemies[i].render(); //render the object
		}

		for (int i=0; i<bullets.size();i++)
		{
			RM.getTexture(bullets[i].getTextureName()).useTexture(); //bind the texture for rendering
			modelMatrix = bullets[i].getTransformMatrix(); //set the model matrix for rendering

			gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &modelMatrix[0][0]);

			//================================================
			//RENDER
			//================================================

			bullets[i].render(); //render the object
		}
		
		for (int i=0; i<pickupObject.size();i++)
		{
			RM.getTexture(pickupObject[i].getTextureName()).useTexture(); //bind the texture for rendering
			modelMatrix = pickupObject[i].getTransformMatrix(); //set the model matrix for rendering

			gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &modelMatrix[0][0]);

			pickupObject[i].render(); //render the object
		}

		for (int i=0; i<bonusObject.size();i++)
		{
			RM.getTexture(bonusObject[i].getTextureName()).useTexture(); //bind the texture for rendering
			modelMatrix = bonusObject[i].getTransformMatrix(); //set the model matrix for rendering

			gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &modelMatrix[0][0]);

			bonusObject[i].render(); //render the object
		}
}

void renderMenu()
{
	//clear depth buffer before rendering
	gl::Clear(gl::DEPTH_BUFFER_BIT);
	//clear color buffer before rendering
	gl::Clear(gl::COLOR_BUFFER_BIT);

	RM.getTexture(objects[0].getTextureName()).useTexture(); //bind the texture for rendering
	RM.getShader(objects[0].getShaderName()).useProgram(); //ready the shader for rendering
	modelMatrix = objects[0].getTransformMatrix(); //set the model matrix for rendering

	//set up the model, view and porjection matrix for use with the shader
	GLuint modelMatrixID = gl::GetUniformLocation(RM.getShader(objects[0].getShaderName()).getProgramID(), "mModel");
	GLuint viewMatrixID = gl::GetUniformLocation(RM.getShader(objects[0].getShaderName()).getProgramID(), "mView");
	GLuint projectionMatrixID = gl::GetUniformLocation(RM.getShader(objects[0].getShaderName()).getProgramID(), "mProjection");

	gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, &modelMatrix[0][0]);
	gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, &player1.playerCam.getViewMatrix()[0][0]);
	gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, &player1.playerCam.getProjectionMatrix()[0][0]);


	objects[0].render();
}

void spawnPickup(){

	pickups newPickup;
	a = rand()%36 - 18;
	b = rand()%36 - 18;

	
	newPickup.setUpPickupObject(RM.getMesh("block"), "health.png", "shader", glm::vec3(a,0.0,b));
	newPickup.scale(glm::vec3(0.5,0.5,0.5));
	pickupObject.push_back(newPickup);


}

void spawnBonus(){

	pickups newBonus;
	a = rand()%38 - 15;
	b = rand()%38 - 15;
	
	newBonus.setUpPickupObject(RM.getMesh("block"), "fist.png", "shader", glm::vec3(a,5.0,b));
	newBonus.setVel(glm::vec3(0,2,0));
	pickupObject.push_back(newBonus);


}

void spawnEnemy(){
	enemy newEnemy;

	float x, z;
	float xDif, zDif;
	float dist = 0;


	while(dist < 10){
		
		x = rand()%36 - 18;
		z = rand()%36 - 18;

		xDif = player1.getPosition().x - x;
		zDif = player1.getPosition().z - z;

		dist = sqrt((xDif*xDif)+(zDif*zDif));
	}

	newEnemy.setUpEnemyObject(RM.getMesh("block"), "skull.jpg", "shader", glm::vec3(x,0.0,z));

	enemies.push_back(newEnemy);
}

void sounds(){
	if(killCounter == 5){
		SoundEngine->play2D("../Resources/Killing Spree.mp3", gl::FALSE_);
		player1.score += 100;
	}
	else if(killCounter == 10){
		SoundEngine->play2D("../Resources/Killing Frenzy.mp3", gl::FALSE_);
		player1.score += 200;
	}
	else if(killCounter == 15){
		SoundEngine->play2D("../Resources/Running Riot.mp3", gl::FALSE_);
		player1.score += 300;
	}
	else if(killCounter == 25){
		SoundEngine->play2D("../Resources/Untouchable.mp3", gl::FALSE_);
		player1.score += 500;
	}
	else if(killCounter == 35){
		SoundEngine->play2D("../Resources/Inconceivable.mp3", gl::FALSE_);
		player1.score += 750;
	}
	else if(killCounter == 45){
		SoundEngine->play2D("../Resources/Invincible.mp3", gl::FALSE_);
		player1.score += 1000;
	}
	else if(killCounter == 55){
		SoundEngine->play2D("../Resources/Unfrigginbelievable.mp3", gl::FALSE_);
		player1.score += 1500;
	}

}

void updateBonus(){
	for(int i = 0; i < bonusObject.size(); i++){
		bonusObject[i].setPosition(bonusObject[i].getPosition() + bonusObject[i].getVel());
		for (int j = 0; j < enemies.size(); j++){

			if (coll.checkCollision(player1,bonusObject.at(j)) == true){
					bonusObject.erase(bonusObject.begin() +j);
					j--;
					player1.health = 100;
					std::cout << "player health: " << player1.health << std::endl;
				}
			}
			
		}
	}

void updateBullets(){
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i].setPosition(bullets[i].getPosition() + bullets[i].getVel());
		for (int j = 0; j < enemies.size(); j++){
				if (coll.checkCollision(bullets[i], enemies[j])){
					bullets.erase(bullets.begin() + i);
					enemies.erase(enemies.begin() + j);
					j = enemies.size();
					i--;
					player1.score += 100;
					killCounter += 1;
					sounds();
					std::cout << "Score: " << player1.score << std::endl;
				}
			}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		if (abs(bullets[i].getPosition().x) > 20 || bullets[i].getPosition().y < -10 || abs(bullets[i].getPosition().z > 20)){
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	//=====================================================================================
	//Initialize GLFW and openGL
	//=====================================================================================

	// Initialize GLFW
	if(!glfwInit()) 
	exit( EXIT_FAILURE );

	// Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, TRUE);

	//=====================================================================================
	//Create the window
	//=====================================================================================

	GLint height = 720; //height of the window
	GLint width = 1280; //width of the window
	std::string title = "Game Engine"; //name of the window

	window = glfwCreateWindow(width,height,title.c_str(), NULL, NULL); //create the render window
	
	//check window has opened
	if(window == NULL){
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;}

	//set the created window to the one that should be used
	glfwMakeContextCurrent(window);

	// Load the OpenGL functions.
    gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
    if (!didLoad) {
		glfwTerminate();
		exit(EXIT_FAILURE);}

	//make the window blue
	gl::ClearColor(0.0,0.5,1.0,1.0);
	//dont render faces that cant be seenenemy
	gl::Enable(gl::CULL_FACE);

	//enable depth testing
	gl::Enable(gl::DEPTH_TEST);
	
	//set the mouse current and old positions
	oldX = 0.0;
	oldY = 0.0;
	posX = 0.0;
	posY = 0.0;

	//=====================================================================================
	//LOAD ASSETS
	//=====================================================================================

	RM.loadMesh("block.txt", "block"); //create a block
	RM.loadMesh("Plane.txt", "plane"); //create a block
	RM.loadMesh("window.txt", "window"); //create a block
	RM.loadMesh("skybox.txt", "skybox"); //create a block

	RM.loadShader("../shader/basic3.frag","../shader/basic3.vert", "skyboxShader"); //load a shader
	RM.loadShader("../shader/basic4.frag","../shader/basic4.vert", "shader"); //load a shader
	RM.loadShader("../shader/basic5.frag","../shader/basic5.vert", "menuShader"); //load a shader
	RM.loadShader("../shader/basic6.frag","../shader/basic6.vert", "spotShader"); //load a shader

	RM.loadTexture("fist.png"); //load a texture
	RM.loadTexture("health.png"); //load a texture
	RM.loadTexture("face.png"); //load a texture
	RM.loadTexture("grass.png"); //load a texture
	RM.loadTexture("Splash.png"); //load a texture
	RM.loadTexture("Menu.png"); //load a texture
	RM.loadTexture("texture1.png"); //load a texture
	RM.loadTexture("texture2.png"); //load a texture
	RM.loadTexture("texture3.png"); //load a texture
	RM.loadTexture("texture4.png"); //load a texture
	RM.loadTexture("wall.jpg");
	RM.loadTexture("skull.jpg");
	RM.loadTexture("red.jpg");
	RM.loadTexture("skybox_texture.jpg"); //load a texture

	//create objects to use
	gameObject plane(RM.getMesh("plane"), "grass.png", "shader", glm::vec3(0.0,-1.0,0.0));
	gameObject skybox(RM.getMesh("skybox"), "skybox_texture.jpg", "skyboxShader", glm::vec3(0,0,0)); //set the skybox position

	gameObject wall1(RM.getMesh("block"), "wall.jpg", "shader", glm::vec3(0.0,-5.0,20.0));
	gameObject wall2(RM.getMesh("block"), "wall.jpg", "shader", glm::vec3(0.0,-5.0,-20.0));
	gameObject wall3(RM.getMesh("block"), "wall.jpg", "shader", glm::vec3(20.0,-5.0,0.0));
	gameObject wall4(RM.getMesh("block"), "wall.jpg", "shader", glm::vec3(-20.0,-5.0,0.0));

	wall1.scale(glm::vec3(40,25,1));
	wall2.scale(glm::vec3(40,25,1));
	wall3.scale(glm::vec3(1,25,40));
	wall4.scale(glm::vec3(1,25,40));


	gameObject menu(RM.getMesh("window"), "Menu.png", "menuShader", glm::vec3(0.0,0.0,0.0));
	gameObject splash(RM.getMesh("window"), "Splash.png", "menuShader", glm::vec3(0.0,0.0,-10.0));
	
	objects.push_back(menu);
	objects.push_back(skybox);

	objects.push_back(wall1);
	objects.push_back(wall2);
	objects.push_back(wall3);
	objects.push_back(wall4);
	objects.push_back(plane); //create a plane for the world

	player1.setUpPlayerObject(RM.getMesh("block"), "texture4.png", "shader", glm::vec3(0.0,0.0,5.0));
	player1.setUpPlayerCamera(70.0f,0.1f,500.0f,16.0f/9.0f,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f));

	gamemode = 2;

	//=====================================================================================
	//GAME LOOP
	//=====================================================================================
	float elapsedTime = glfwGetTime();
	//while the renderwindow isnt closed, keep running through the loop
	while(!glfwWindowShouldClose(window))
	{
		GLint width,height;
		//get the size of the framebuffer of the current context (current window)
		glfwGetFramebufferSize(window, &width, &height);
		//set the viewport size
		gl::Viewport(0,0,width,height);
		//clear color buffer before rerendering
		gl::Clear(gl::COLOR_BUFFER_BIT);

		if(loaded == false && glfwGetTime() > 0)
		{
			gamemode = 3;
			loaded  = true;
			std::cout << "LOADED" << std::endl;
		}
		

		if(glfwGetTime()-elapsedTime > 0)
		{
		//=====================
		// MAIN LOOP GOES HERE
		//=====================
		 //get and proecss input
		glm::vec3 previousPosition = player1.playerCam.getPosition();
		player1.setPosition(player1.playerCam.getPosition());
		input();
		player1.setPosition(player1.playerCam.getPosition());
	
		if(player1.health <= 0){

			if (player1.score > highscore) {
				highscore = player1.score; 
				std::cout << "New High Score: " << highscore << "!" << std::endl;
			}

			SoundEngine->play2D("../Resources/Game Over.mp3", gl::FALSE_);
			gamemode = 3;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwGetCursorPos(window,&posX,&posY); //get the position of the cursor within the window
			player1.reset(glm::vec3(0.0,0.0,5.0));
			
			}


		if(gamemode == 1)
		{
			for(int i = 2; i < objects.size(); i ++)
			{
				if (coll.checkCollision(player1,objects.at(i)) == true)
				{
	
					player1.playerCam.setPosition(previousPosition);
					
				}
			}

			//MOVE ALL THE TIME
			move.euler(&player1.playerCam);
			player1.setPosition(player1.playerCam.getPosition());
			
			for(int i = 2; i < objects.size(); i ++)
			{
				if (coll.checkCollision(player1,objects.at(i)) == true)
				{
					player1.setCanJump(true);
					float dif = objects.at(i).bb.getMax().y - player1.bb.getMin().y;
					float height = (player1.bb.getMax().y - player1.bb.getMin().y)/2;
					player1.playerCam.setPosition(glm::vec3(player1.playerCam.getPosition().x,previousPosition.y,player1.playerCam.getPosition().z));
					player1.playerCam.setVelocity(glm::vec3(0,0,0));
					player1.setPosition(player1.playerCam.getPosition());
				}
			}



			//CHASE
			for (int i = 0; i < enemies.size(); i++){
				if (enemies[i].chase(player1) == true){}
				else {
					enemies.erase(enemies.begin() + i);
					i--;
					player1.health -= 10;
					killCounter = 0;
					std::cout << "player health: " << player1.health << std::endl;
					SoundEngine->play2D("../Resources/exp1.mp3", gl::FALSE_);
				}
			}

			updateBullets();

			spawnTimer = glfwGetTime();
			if (spawnTimer - startTime >  enemiesSpawned*1.5){
				spawnEnemy();
				enemiesSpawned++;
			}
			for(int j = 0; j < pickupObject.size(); j++){
				if (coll.checkCollision(player1,pickupObject.at(j)) == true){
					pickupObject.erase(pickupObject.begin() +j);
					j--;
					if (player1.health < 100) player1.health += 10;
					std::cout << "player health: " << player1.health << std::endl;
				}
			}

			pickupTimer = glfwGetTime();

			if (pickupTimer - timer2 >  pickupsSpawned*6){
				
				if (pickupObject.size() < 1) spawnPickup();
				pickupsSpawned++;
				
			}

	

			/*for(int h = 0; h < bonusObject.size(); h++){
				if (coll.checkCollision(player1,pickupObject.at(h)) == true){
					bonusObject.erase(bonusObject.begin() +h);
					h--;
					player1.health = 100;
					std::cout << "player health: " << player1.health << std::endl;
				}
			}*/
			
			/*updateBonus();

			bonusTimer = glfwGetTime();
			if(bonusTimer - timer3 >bonusSpawned*2 && player1.health <= 20 && bonusSpawned != 2){
				spawnBonus();
				
				bonusSpawned++;
			}*/




			objects.at(1).setPosition(glm::vec3(player1.getPosition().x,player1.getPosition().y,player1.getPosition().z));
			render(); //render all the objects
		}
	

		if(gamemode == 3)
		{
			renderMenu(); //render the menu
		}



		//get inputs
		glfwPollEvents();
		//redraws the window
		glfwSwapBuffers(window);

		elapsedTime = glfwGetTime();
		}
	}
	//destroy window
	glfwDestroyWindow(window);
	//terminate glfw
	glfwTerminate();
	exit(0);
	
}

