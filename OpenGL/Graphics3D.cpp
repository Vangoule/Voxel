#include <GL/glew.h>
#include "Graphics3D.h"
#include "Globals.h"
#include "shader.hpp"

#include <stdio.h>
#include <time.h>

#define ILUT_USE_OPENGL	// This MUST be defined before calling the DevIL headers or we don't get OpenGL functionality
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "Model.h"

#include "Chunk.h"
#include "ChunkManager.h"
#include <SFML/System.hpp>
#include <thread>
#include <future>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF



GLuint programID;
GLuint TextureID;
GLuint MatrixID;

glm::mat4 Projection;
glm::mat4 View ;
glm::mat4 MVP;

Graphics3D::Graphics3D(void)
{
	load = false;
}

glm::mat4 Graphics3D::getMVP()
{
	return MVP;
}
glm::mat4 Graphics3D::getProjection()
{
	return Projection;
}

glm::mat4 Graphics3D::getView()
{
	return View;
}

GLuint Graphics3D::getTextureID()
{
	return TextureID;
}


GLuint Graphics3D::getMatrixID()
{
	return MatrixID;
}


GLuint Graphics3D::loadTexture(const char* theFileName)
{
	ILuint imageID;			

	GLuint textureID;		

	ILboolean success;

	ILenum error;				

	ilGenImages(1, &imageID); 	

	ilBindImage(imageID); 		

	success = ilLoadImage(theFileName); 

	if (success)
	{

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			//		iluFlipImage();
		}

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);


		if (!success)
		{
			error = ilGetError();
			std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
		}


		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);


		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_BPP),	// Image colour depth
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Image format (i.e. RGB, RGBA, BGR etc.)
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
		*/

		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if(GLEW_EXT_texture_filter_anisotropic)
		{

			GLfloat maximumAnisotropy;
			//get the value
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnisotropy);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); 
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
		glGenerateMipmap(GL_TEXTURE_2D);


	}
	else 
	{
		error = ilGetError();
		std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;

	}

	ilDeleteImages(1, &imageID); 

	std::cout << "Texture creation successful." << std::endl;

	return textureID; 

}

GLuint Texture;
ChunkManager chunkManager;

void Graphics3D::Init()
{

	std::cout << "Initiating Graphics" << std::endl << std::endl;

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	//  ----- Initialise DevIL -----
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);


	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH); 
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);


	// Enable depth test
	//   glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//   glDepthFunc(GL_LESS); 

	programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
	MatrixID = glGetUniformLocation(programID, "MVP");
	TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	/* Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	// Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 500.0f);
	// Camera matrix
	View       = glm::lookAt(
	glm::vec3(0,6,-15), // Camera is at (4,3,3), in World Space
	glm::vec3(0,0,0), // and looks at the origin
	glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	*/
	glm::mat4 Model      = glm::mat4(1.0f);

	MVP        = Projection * View * Model;


	Texture = loadTexture("texture.png");

	srand ( time(NULL) );

	noiseManager.CreateHeightmap();
	//	chunkManager.CreateChunk(0, 0);
	//	chunkManager.SaveChunk(0, 0);
	//chunkManager.CreateChunk(0,0);

}

int i = 0;
int j = 0;

std::mutex mutex;

void Graphics3D::Run(int i, int j)
{
	if(i == 0)
	{
		std::cout << "THIS THREAD = " << std::this_thread::get_id() << std::endl;
		std::lock_guard<std::mutex> guard(mutex);
		chunkManager.CreateChunk(i,j);
		std::cout << "DONE" << std::endl;
	}

}


std::future<void> g;


void Graphics3D::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.f,0.f,0.f,0.f);

	glUseProgram(programID);

	glEnable(GL_CULL_FACE);


	//chunkManager.Update();

	if(load == true)
	{
		chunkManager.CreateChunk(i,j);
		//result=( std::async([](int x, int y, ChunkManager* cMan) { std::cout << "THREAD RUNNING" << std::endl; cMan->CreateChunk(x, y); } , i, j, &chunkManager));
		//g = std::async([](int x, int y, ChunkManager* cMan) { std::cout << "THREAD RUNNING" << std::endl; cMan->CreateChunk(x, y); } , i, j, &chunkManager);

	//	g = std::async(std::launch::async,&Graphics3D::Run,this,i,j);
		

	//	std::cout << "Thread fired from = " << std::this_thread::get_id() << std::endl;



		i++;
		if(i == 16)
		{
			j++;
			i = 0;
		}

		load = false;
	}


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);


	chunkManager.RenderChunks();


	glBindTexture(GL_TEXTURE_2D, 0); 
	engine->getWindowHandle()->display();

}


