#include "stdafx.h"
#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "ResourceManager.h"

resourceManager::resourceManager()
{
}

void resourceManager::loadMesh(std::string path, std::string meshName)
{
	mesh Mesh; //create an instance of mesh
	std::string texName; // a string to hold the name of the texture when reading the file in

	glm::vec3 tempVert; //a temporary vec3 to hold the vertices as they are read in
	glm::vec3 tempNorm; //a temporary vec3 to hold the normals as they are read in
	glm::vec2 tempUV; //a temporary vec2 to hold the UVs as they are read in

	std::vector<glm::vec3> tempvertices; // a temporary vector of vertices, holds the vertices for use when indexing
	std::vector<glm::vec2> tempUVs; // a temporary vector of UVs, holds the UVs for use when indexing
	std::vector<glm::vec3> tempNormals; // a temporary vector of normals, holds the normals for use when indexing

	//open the file to read
	std::ifstream meshFile; //create an inpur file stream
	meshFile.open(path); //open the file using the path 
	std::string line; //create a string to hold each line of the file

	
	while(getline(meshFile,line)) //while a line can be read in
	{
		std::stringstream in(line); //read in a line
		in>>line;

		//read in vertex
		if(line == "v") //if a line starts with v its a vertex
		{
			glm::vec3 vertex; //vreate a glm vec3 for the vertex
			in>>vertex.x>>vertex.y>>vertex.z; //read the three values in the line into the values of vertex
			tempvertices.push_back(vertex); //add the vertex to the temporary vector of vertices
		}

		//read in a  UV value
		if(line=="vt") //if the line starts with vt its a uv coordinate
		{
			glm::vec2 uv; //create a glm vec2 to hold the UV
			in>>uv.x>>uv.y; //rad in both the uv values on the line into the vector
			tempUVs.push_back(uv); //push the uv back onto the temporary uv vector
		}

		//read in a normal
		if(line == "vn") //if the line starts with vn its a normal
		{
			glm::vec3 normal; //create a vec3 to hold the normal
			in>>normal.x>>normal.y>>normal.z; //read the 3 normal values into the normal vector
			tempNormals.push_back(normal); //add the normal vector onto the temporary vector of normals
		}

	//Read in the indices
		if(line == "f") //if the line starts with f its a face of the mesh
		{
			glm::vec3 face; // the vertex, uv and normal of a point of the face
			char ignore; //a char used to hold the '/' between each face value
			
			//3 points are on each line so repeat the indexing 3 times per line
			for(int i=0; i<3; i++)
			{
				//read in the values of the first face point
				in>>face.x>>ignore>>face.y>>ignore>>face.z;

				//get the vertex in the tempVertices vector at the point of the first face value
				tempVert = tempvertices.at(face.x-1);
				//get the UV in the tempUVs vector at the point of the second face value
				tempUV = tempUVs.at(face.y-1);
				//get the normal in the tempNormals vector at the point of the third face value
				tempNorm = tempNormals.at(face.z-1);

				//=========================================================================
				//INDEXING THE VERTICES, NORMALS AND UVS
				//=========================================================================

				//if no values have been added to the vertex vector
				if(Mesh.getVertices().size() == 0)
				{
					Mesh.addVertex(tempVert); // add the vertex in tempVertex to the mesh vertex vector
					Mesh.addUV(tempUV); // add the UV in tempUV to the mesh UV vector
					Mesh.addNormal(tempNorm); // add the normal in tempNormal to the mesh normal vector
					Mesh.addIndex(0); // add the 0 to the mesh index vector
				}

				//if there is a value in the mesh vertex vector
				else
				{
					bool match = false; // a bool to say if any vertex, normal and UV match exactly with any in matching positions in the mesh vector.
										//e.g. if the vertex, UV and normal in position 4 of their respective vector match the three temporary values,
										// then match is true, if not then its false
					int position = 0; // the number of values in the vertex vector
					for(int i = 0; i < Mesh.getVertices().size(); i++) //for each value in the vertex vector
					{

						if(tempVert == Mesh.getVertices().at(i) && tempUV == Mesh.getUV().at(i) && tempNorm == Mesh.getNormal().at(i))
						{
							//if the tempVertex matches the mesh vertex, the tempUV matches 
							//the mesh UV and the temp normal matches the mesh normal
							position = i; //position is equal to the current position in the vertex vector
							match = true; //there was a match
						}
					}

					//if there was a match
					if(match == true)
					{
						Mesh.addIndex(position); //add the position in the vertex vector to the index vector
												 //so that the renderer knows to use the vertex, normal and uv combination for that face point
					}
					//if there wasnt a match
					else
					{
						Mesh.addVertex(tempVert); //add the temporary vertex to the vertex vector
						Mesh.addUV(tempUV); //add the temporary UV to the UV vector
						Mesh.addNormal(tempNorm); //add the temporary normal to the normal vector
						Mesh.addIndex(Mesh.getVertices().size()-1); // add the size of vertcies-1 (the index position of each new value) to the index vector
					}
				}
			}
		}

		if(line == "usemtl") //if the line starts with usemtl
		{
			//read the name of the meshes texture into texName
			in>>texName;
			Mesh.addTexName(texName); //set the name of the meshes texture to texName
		}
	}

	meshFile.close(); // close the .obj file after reading it in
	this->meshes[meshName] = Mesh; //add the mesh to the map of meshes
}

void resourceManager::loadTexture(std::string textureName)
{
	GLuint textureID; //the id for the texture
	gl::GenTextures(1, &textureID); //generate a single id

	int width; //the width of the texture
	int height; //the height of the texture
	int comp; //components per pixel

	//create the full path from the name of the texture
	std::string texturePath = "../Resources/"+ textureName;

	//load in the image
	unsigned char* image = stbi_load(texturePath.c_str(), &width, &height, &comp, STBI_rgb_alpha);
	
	//create a new texture object
	texture newTexture(textureID,width,height);

	//make the texture active
	gl::BindTexture(gl::TEXTURE_2D, textureID);
	
	//set the parameters of the texture
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST); //min filter for when pixel is larger than texture element, linear uses texture value nearest pixel
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST); //mag filter for when pixel is smaller than texture element, linear uses texture value nearest pixel
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::LINEAR); //sets paramter for coordinate s. linear is the weighted average of 4 elements
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::LINEAR); //sets parameter for coordinate t
	
	//create a 2d image
	gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, width, height, 0, gl::RGBA,gl::UNSIGNED_BYTE, image);

	//free up image and unbind the texture
	gl::BindTexture(gl::TEXTURE_2D, 0);
	stbi_image_free(image);

	//add the texture to the texture map
	this->textures[textureName] = newTexture;
}

void resourceManager::loadShader(std::string frag, std::string vert, std::string name)
{
	//create the program
	GLuint programID = gl::CreateProgram();
	 if(0 == programID) {
        fprintf(stderr, "Error creating program object.\n");
        exit(1);
    }

	 shader Shader(programID);

	//======================================================================
	//LOAD FRAGMENT SHADER
	//======================================================================
	GLuint fragID = gl::CreateShader(gl::FRAGMENT_SHADER);
	if (0 == fragID) {
      std::cout << "Error creating vertex shader" << std::endl;
	  //exit(EXIT_FAILURE);
	}

	/////////////////////////
	// Load contents of file 
	/////////////////////////
    std::ifstream fragFile(frag);
    if (!fragFile) {
        std::cout << "Error opening file: " << frag << std::endl;
       // exit(EXIT_FAILURE); 
	}

    std::stringstream fragcode;
    fragcode << fragFile.rdbuf();
	fragFile.close();
	std::string fragcodeStr(fragcode.str());

	////////////////////////////
    // Create the shader object
	/////////////////////////////
    if (0 == fragID) {
      std::cout << "Error creating fragment shader" << std::endl;
	 // exit(EXIT_FAILURE);
	}

	///////////////////////////////////////////////
    // Load the source code into the shader object
	////////////////////////////////////////////////
    const GLchar* fragcodeArray[] = {fragcodeStr.c_str()};
    gl::ShaderSource(fragID, 1, fragcodeArray, NULL);

	///////////////////////
    // Compile the shader
	////////////////////////
	gl::CompileShader(fragID);

    // Check compilation status
    GLint fragresult;
    gl::GetShaderiv(fragID, gl::COMPILE_STATUS, &fragresult );
    if( FALSE == fragresult ) {
       fprintf( stderr, "fragment shader compilation failed!\n" );

       GLint logLen;
       gl::GetShaderiv(fragID, gl::INFO_LOG_LENGTH, &logLen );

       if (logLen > 0) {
           char * log = (char *)malloc(logLen);

           GLsizei written;
           gl::GetShaderInfoLog(fragID, logLen, &written, log);

           fprintf(stderr, "Shader log: \n%s", log);

           free(log);
       }
    }
	
	//======================================================================
	//LOAD VERTEX SHADER
	//======================================================================

	GLuint vertID = gl::CreateShader(gl::VERTEX_SHADER);
	if (0 == vertID) {
      std::cout << "Error creating vertex shader" << std::endl;
	  //exit(EXIT_FAILURE);
	}

	/////////////////////////
	// Load contents of file
	/////////////////////////
    std::ifstream vertFile(vert);
    if (!vertFile) {
        std::cout << "Error opening file: " << vert << std::endl;
        //exit(EXIT_FAILURE); 
	}

    std::stringstream vertcode;
    vertcode << vertFile.rdbuf();
	vertFile.close();
	std::string vertcodeStr(vertcode.str());

	///////////////////////////////////////////////
    // Load the source code into the shader object
	////////////////////////////////////////////////
    const GLchar* vertcodeArray[] = {vertcodeStr.c_str()};
    gl::ShaderSource(vertID, 1, vertcodeArray, NULL);

	///////////////////////
    // Compile the shader
	////////////////////////
	gl::CompileShader(vertID);

    // Check compilation status
    GLint result;
    gl::GetShaderiv(vertID, gl::COMPILE_STATUS, &result );
    if( FALSE == result ) {
       fprintf( stderr, "Vertex shader compilation failed!\n" );

       GLint logLen;
       gl::GetShaderiv(vertID, gl::INFO_LOG_LENGTH, &logLen );

       if (logLen > 0) {
           char * log = (char *)malloc(logLen);

           GLsizei written;
           gl::GetShaderInfoLog(vertID, logLen, &written, log);

           fprintf(stderr, "Shader log: \n%s", log);

           free(log);
       }
    }

	//======================================================================
	//ATTACH THE SHADERS
	//======================================================================
	gl::AttachShader(programID, fragID);
	gl::AttachShader(programID, vertID);

	//======================================================================
	//LINK THE SHADER PROGRAM
	//======================================================================

	// Link the program
    gl::LinkProgram(programID);

    // Check for successful linking
    GLint status;
    gl::GetProgramiv(programID, gl::LINK_STATUS, &status );
    if (FALSE == status) {
        std::cout << "Failed to link shader program!\n" <<std::endl;

        GLint logLen;
        gl::GetProgramiv(programID, gl::INFO_LOG_LENGTH, &logLen );

        if (logLen > 0) {
            char * log = (char *)malloc(logLen);

            GLsizei written;
            gl::GetProgramInfoLog(programID, logLen, &written, log);

            fprintf(stderr, "Program log: \n%s", log);

            free(log);
        }
    }

	this->shaders[name] = Shader; //add the mesh to the map of meshes
}

mesh& resourceManager::getMesh(std::string meshName)
{
	return meshes.at(meshName); //returns the mesh in the mesh map of name meshName
}

shader& resourceManager::getShader(std::string shaderName)
{
	return shaders.at(shaderName); //return the shader in the shader map of name shaderName
}

texture& resourceManager::getTexture(std::string textureName)
{
	return textures.at(textureName); //return the texture in the texture map of name textureName
}