#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "NoiseManager.h"


class Graphics3D
{
private:
	NoiseManager noiseManager;
public:
	Graphics3D(void);
	void Init();
	void Draw();
	void Load();
	void Run(int i, int j);
	GLuint loadTexture(const char* theFileName);
	glm::mat4 getMVP();
	GLuint getVertexBuffer();
	GLuint getUvBuffer();
	GLuint getMatrixID();
	GLuint getTextureID();

	GLuint getTerrainTextureID(int);
	GLuint getTerrainMatrixID();
	GLuint getTerrainModelMatrixID();
	GLuint getTerrainViewMatrixID();
	GLuint getTerrainLightID();

	GLuint getLightTextureID();
	GLuint getLightMatrixID();
	GLuint getLightModelMatrixID();
	GLuint getLightViewMatrixID();
	GLuint getLightLightID();
	
	glm::mat4 getProjection();
	glm::mat4 getView();

	NoiseManager* getNoiseManager()
	{
		return &noiseManager;
	}

	bool load;



};


