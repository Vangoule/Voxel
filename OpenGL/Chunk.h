#pragma once
#include "Block.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include "Globals.h"

class Chunk
{
public:
    Chunk();
    ~Chunk();

    void Update();

    void Render();

	void CreateMesh();

	void RebuildMesh();

	void Init();

	void Load();
	
	bool isLoaded();
	bool isSetup();

	void MoveRotateScale(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	void CreateCube(int x, int y, int z, bool activeStates[], int ID );

    static const int CHUNK_SIZE_X = 16;
	static const int CHUNK_SIZE_Y = 256;
	static const int CHUNK_SIZE_Z = 16;

	static const int CHUNK_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

	int worldBlocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	glm::vec3 chunkPosition;
private:
    // The blocks data
	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec3> normalData;
	std::vector<glm::vec2> uvData;

	bool loadedState;
    
	float BLOCK_RENDER_SIZE;
	
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;


};