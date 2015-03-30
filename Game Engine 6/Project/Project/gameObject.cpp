#include "stdafx.h"
#include "gameObject.h"

gameObject::gameObject()
{
}

gameObject::gameObject(mesh Mesh, std::string textureName, std::string shaderName, glm::vec3 Position)
{
	this->velocity = 0.f;
	this->Mesh = &Mesh; //set the mesh for the object to the mesh
	this->textureName = textureName; //set the name of the objects texture
	this->shaderName = shaderName; //set the name of the objects shader
	this->position = Position; //set the current position of the object within the world
	this->vel = glm::vec3(0,0,0);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(position)); //set up the transform matrix
	createVAO(Mesh); //create the vao for the object when it is created
	setMinMax();
}

gameObject::gameObject(mesh Mesh, glm::vec3 color, std::string shaderName, glm::vec3 Position)
{
	this->Mesh = &Mesh; //set the mesh for the object to the mesh
	this->color = color; //set the name of the objects texture
	this->shaderName = shaderName; //set the name of the objects shader
	this->position = Position; //set the current position of the object within the world
	this->vel = glm::vec3(0,0,0);
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(position)); //set up the transform matrix
	createVAO(Mesh); //create the vao for the object when it is created
	setMinMax();
}

void gameObject::render()
{
	gl::BindVertexArray(vaoHandle);//bind the vao ready for use when rendering
	gl::DrawElements(gl::TRIANGLES, 50, gl::UNSIGNED_INT, NULL); //draw the elements in the vao
}

void gameObject::createVAO(mesh Mesh)
{
	gl::GenBuffers(3, vboHandle); //generate three ids for the VBOs
    GLuint position = vboHandle[0]; //the position data is in the first vbo
    GLuint UV = vboHandle[1]; //the uv data will be in the second vbo
	GLuint normal = vboHandle[2]; //the normal data will be in the third vbo

	gl::BindBuffer(gl::ARRAY_BUFFER, position); //bind the position vbo ready for use
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec3)*Mesh.getVertices().size(), &Mesh.getVertices().front(), gl::STATIC_DRAW); //add the position data to the vbo

	gl::BindBuffer(gl::ARRAY_BUFFER, UV); //bind the uv vbo for use
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec2)*Mesh.getUV().size(), &Mesh.getUV().front(), gl::STATIC_DRAW); //add the uv data to the vbo

	gl::BindBuffer(gl::ARRAY_BUFFER, normal); //bind the position vbo ready for use
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec3)*Mesh.getNormal().size(), &Mesh.getNormal().front(), gl::STATIC_DRAW); //add the position data to the vbo

	gl::GenVertexArrays(1, &vaoHandle); //generate an ID for the vao
    gl::BindVertexArray(vaoHandle); //bind the vao ready for use

    gl::EnableVertexAttribArray(0); //enable the attribute array for the position data
    gl::EnableVertexAttribArray(1);  //enable the attribute array for the UV data
	gl::EnableVertexAttribArray(2); //enable the attribute array for the normal data

    gl::BindBuffer(gl::ARRAY_BUFFER, position); //bind the position vbo ready for use
    gl::VertexAttribPointer( 0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL ); //set up the attribute pointer for the position data

    gl::BindBuffer(gl::ARRAY_BUFFER, UV);//bind the uv vbo for use
    gl::VertexAttribPointer( 1, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL );//set up the attribute pointer for the UV data

	gl::BindBuffer(gl::ARRAY_BUFFER, normal);//bind the normal vbo for use
    gl::VertexAttribPointer( 2, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL );//set up the attribute pointer for the normal data

	GLuint indexBuffer; //ID for the index buffer
	gl::GenBuffers(1,&indexBuffer); //generate an id for the index buffer
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indexBuffer); //bind the index buffer ready for use
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*Mesh.getIndex().size(), &Mesh.getIndex().front(), gl::STATIC_DRAW);//add the index data to the vbo
}

std::string gameObject::getShaderName()
{
	return shaderName; //return the name of the objects shader
}

std::string gameObject::getTextureName()
{
	return textureName; //return the name of the texture
}

glm::mat4 gameObject::getTransformMatrix()
{
	return transform; //return the transform matrix of the object
}

void gameObject::scale(glm::vec3 scale)
{
	transform = transform*glm::scale(scale);
	bb.setMin(glm::vec3(bb.getMin().x * scale.x, bb.getMin().y * scale.y, bb.getMin().z * scale.z));
	bb.setMax(glm::vec3(bb.getMax().x * scale.x, bb.getMax().y * scale.y, bb.getMax().z * scale.z));
}

void gameObject::setMinMax()
{
	glm::vec3 min;
	glm::vec3 max;
	bool first = true;

	for(int i = 0; i < Mesh->getVertices().size(); i++)
	{
		if(first == true)
		{
			min = Mesh->getVertices().at(i);
			max = Mesh->getVertices().at(i);
			first = false;
		}

		if(Mesh->getVertices().at(i).x < min.x)
			min.x = Mesh->getVertices().at(i).x;
		if(Mesh->getVertices().at(i).y < min.y)
			min.y = Mesh->getVertices().at(i).y;
		if(Mesh->getVertices().at(i).z < min.z)
			min.z = Mesh->getVertices().at(i).z;

		if(Mesh->getVertices().at(i).x > max.x)
			max.x = Mesh->getVertices().at(i).x;
		if(Mesh->getVertices().at(i).y > max.y)
			max.y = Mesh->getVertices().at(i).y;
		if(Mesh->getVertices().at(i).z > max.z)
			max.z = Mesh->getVertices().at(i).z;
	}

	bb.setMax(max);
	bb.setMin(min);
}

bool gameObject::chase(gameObject player){
	float xDif, zDif, hypDif, ratio, xVel, zVel;

	xDif = this->getPosition().x - player.getPosition().x;
	zDif = this->getPosition().z - player.getPosition().z;

	hypDif = sqrtf((xDif*xDif)+(zDif*zDif));

	ratio = this->velocity/hypDif;

	xVel = ratio * xDif *3;
	zVel = ratio * zDif *3;

	if (hypDif > 2){
		this->setPosition(glm::vec3(this->getPosition().x - xVel, this->getPosition().y, this->getPosition().z - zVel));
	}

	

	/*if (xDif > 5) { this->setPosition(glm::vec3(this->getPosition().x - this->velocity, this->getPosition().y, this->getPosition().z)); }
	if (xDif < -5) { this->setPosition(glm::vec3(this->getPosition().x + this->velocity, this->getPosition().y, this->getPosition().z)); }

	if (zDif > 5) { this->setPosition(glm::vec3(this->getPosition().x, this->getPosition().y, this->getPosition().z - this->velocity)); }
	if (zDif < -5) { this->setPosition(glm::vec3(this->getPosition().x, this->getPosition().y, this->getPosition().z + this->velocity)); }*/

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(this->getPosition()));

	if (hypDif < 2){
		//this->setPosition(glm::vec3(this->getPosition().x + xVel, this->getPosition().y, this->getPosition().z + zVel));
		return false;
	}

	return true;

	//createVAO(*Mesh);
}

glm::vec3 gameObject::getPosition()
{
	return position;
}

glm::vec3 gameObject::getVel()
{
	return vel;
}

void gameObject::setPosition(glm::vec3 position)
{
	this->position = position;
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(position)); //set up the transform matrix
}

void gameObject::setVel(glm::vec3 velocity){
	this->vel = velocity;
}