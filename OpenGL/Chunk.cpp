#include "Chunk.h"


glm::mat4 ProjectionM;
glm::mat4 ViewM;
glm::mat4 ModelM;
glm::mat4 chunkMVP;


Chunk::Chunk()
{
	BLOCK_RENDER_SIZE = 0.5;
	loadedState = false;
}

void Chunk::MoveRotateScale(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{


	glm::mat4 T = glm::translate(glm::mat4(1.0f),pos);
	glm::mat4 Rx = glm::rotate(T, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry = glm::rotate(Rx, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 AlmostModel = glm::rotate(Ry, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 Model = glm::scale(AlmostModel, scale);

	chunkMVP        = engine->getInputManager()->getProjectionMatrix() * engine->getInputManager()->getViewMatrix() * Model;


}

void Chunk::Load()
{
	loadedState = true;
	ModelM      = glm::mat4(1.0f);
	chunkMVP    = engine->getGraphicsManager()->getProjection() * engine->getGraphicsManager()->getView() * ModelM;
}

void Chunk::Init()
{  

	loadedState = true;

	for(int i = 0; i < CHUNK_SIZE_X; i++)
	{
		for(int j = 0; j < CHUNK_SIZE_Y; j++)
		{
			for(int k = 0; k < CHUNK_SIZE_Z; k++)
			{

				worldBlocks[i][j][k] = 0;
			}

		}
	}

	for(int i = 0; i < CHUNK_SIZE_X; i++)
	{
		for(int k = 0; k < CHUNK_SIZE_Z; k++)
		{
			int height = engine->getGraphicsManager()->getNoiseManager()->getHeightAt( (chunkPosition.x ) + i, (chunkPosition.z) + k) * 10;
			//	std::cout << "Height: " << height << std::endl;
			//std::cout << "X: " << (chunkPosition.x ) + i << " Y:" << (chunkPosition.z) + k << std::endl;
			for(int j = 0; j < 125; j++)
			{
				worldBlocks[i][j][k] = 2;
			}
			for(int j = 125; j < 126; j++)
			{
				worldBlocks[i][j][k] = 3;
			}
			for(int j = 126; j < 126+height; j++)
			{
				worldBlocks[i][j][k] = 3;
			}

		}
	}



	ModelM      = glm::mat4(1.0f);
	chunkMVP    = engine->getGraphicsManager()->getProjection() * engine->getGraphicsManager()->getView() * ModelM;
//	std::cout << "Chunk Created with size: " << CHUNK_SIZE << std::endl;
}

Chunk::~Chunk()
{

}

void Chunk::RebuildMesh()
{
	vertexData.clear();
	uvData.clear();
	normalData.clear();

	bool lDefault = false;
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				if(worldBlocks[x][y][z] == 0)
				{
					continue;
				}

				bool lXNegative = lDefault;
				if(x > 0){
					if(worldBlocks[x-1][y][z] != 0)
						lXNegative = true ;
				}
				bool lXPositive = lDefault;
				if(x < CHUNK_SIZE_X - 1){
					if(worldBlocks[x+1][y][z] != 0)
						lXPositive = true ;
				}
				bool lYNegative = lDefault;
				if(y > 0){
					if(worldBlocks[x][y-1][z] != 0)
						lYNegative = true;
				}
				bool lYPositive = lDefault;
				if(y < CHUNK_SIZE_Y - 1){
					if(worldBlocks[x][y+1][z] != 0)
						lYPositive = true ;
				}
				bool lZNegative = lDefault;
				if(z > 0){
					if(worldBlocks[x][y][z-1] != 0)		
						lZNegative = true ;
				}
				bool lZPositive = lDefault;
				if(z < CHUNK_SIZE_Z - 1){
					if(worldBlocks[x][y][z+1] != 0)		
						lZPositive = true ;
				}
				bool activeStates[6];
				activeStates[0] = lXNegative; //left
				activeStates[1] = lXPositive; //right
				activeStates[2] = lYNegative; // bottom
				activeStates[3] = lYPositive; // top
				activeStates[4] = lZNegative; // back
				activeStates[5] = lZPositive; // forward	

				CreateCube(x, y, z, activeStates, worldBlocks[x][y][z]);
			}
		}
	}

	/*	
	for(int i = 0; i < vertexData.size(); i+=3)
	{

	// get the three vertices that make the faces
	glm::vec3 p1 = vertexData[i+0];
	glm::vec3 p2 = vertexData[i+1];
	glm::vec3 p3 = vertexData[i+2];

	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 normal = glm::cross( v1,v2 );

	normal = glm::normalize(normal);

	normalData[i+0] = normal;
	normalData[i+1] = normal;
	normalData[i+2] = normal;

	}
	*/	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(glm::vec2), &uvData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(glm::vec3), &normalData[0], GL_STATIC_DRAW);
}
bool Chunk::isSetup()
{
	if(vertexData.size() != NULL)
		return true;
	else{
		return false;
	}
}

bool Chunk::isLoaded()
{
	return loadedState;	
}
void Chunk::CreateMesh()
{
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &normalBuffer);

	bool lDefault = false;
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				if(worldBlocks[x][y][z] == 0)
				{
					continue;
				}

				bool lXNegative = lDefault;
				if(x > 0){
					if(worldBlocks[x-1][y][z] != 0)
						lXNegative = true ;
				}
				bool lXPositive = lDefault;
				if(x < CHUNK_SIZE_X - 1){
					if(worldBlocks[x+1][y][z] != 0)
						lXPositive = true ;
				}
				bool lYNegative = lDefault;
				if(y > 0){
					if(worldBlocks[x][y-1][z] != 0)
						lYNegative = true;
				}
				bool lYPositive = lDefault;
				if(y < CHUNK_SIZE_Y - 1){
					if(worldBlocks[x][y+1][z] != 0)
						lYPositive = true ;
				}
				bool lZNegative = lDefault;
				if(z > 0){
					if(worldBlocks[x][y][z-1] != 0)		
						lZNegative = true ;
				}
				bool lZPositive = lDefault;
				if(z < CHUNK_SIZE_Z - 1){
					if(worldBlocks[x][y][z+1] != 0)		
						lZPositive = true ;
				}
				bool activeStates[6];
				activeStates[0] = lXNegative; //left
				activeStates[1] = lXPositive; //right
				activeStates[2] = lYNegative; // bottom
				activeStates[3] = lYPositive; // top
				activeStates[4] = lZNegative; // back
				activeStates[5] = lZPositive; // forward	

				CreateCube(x, y, z, activeStates, worldBlocks[x][y][z]);
			}
		}
	}

	/*	
	for(int i = 0; i < vertexData.size(); i+=3)
	{

	// get the three vertices that make the faces
	glm::vec3 p1 = vertexData[i+0];
	glm::vec3 p2 = vertexData[i+1];
	glm::vec3 p3 = vertexData[i+2];

	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 normal = glm::cross( v1,v2 );

	normal = glm::normalize(normal);

	normalData[i+0] = normal;
	normalData[i+1] = normal;
	normalData[i+2] = normal;

	}
	*/	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(glm::vec2), &uvData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(glm::vec3), &normalData[0], GL_STATIC_DRAW);
}

void Chunk::CreateCube(int x, int y, int z, bool activeStates[], int ID)
{

	double TEXTURE_SIZE = 256;
	glm::vec3 p1(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE); //left bottom  front 
	glm::vec2 t1(0.5/TEXTURE_SIZE, 0.5/TEXTURE_SIZE);
	glm::vec3 p2(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE); //right bottom front
	glm::vec2 t2(1-(0.5/TEXTURE_SIZE), 0.5/TEXTURE_SIZE);
	glm::vec3 p3(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE); // right top front
	glm::vec2 t3(1-(0.5/TEXTURE_SIZE), 1-(0.5/TEXTURE_SIZE));
	glm::vec3 p4(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE); // left top front
	glm::vec2 t4(0.5/TEXTURE_SIZE, 1-(0.5/TEXTURE_SIZE));
	glm::vec3 p5(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE); // right bottom back
	glm::vec2 t5(0.5/TEXTURE_SIZE, 0.5/TEXTURE_SIZE);
	glm::vec3 p6(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE); // left bottom back
	glm::vec2 t6(1-(0.5/TEXTURE_SIZE), 0.5/TEXTURE_SIZE);
	glm::vec3 p7(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE); // left top back
	glm::vec2 t7(1-(0.5/TEXTURE_SIZE), 1-(0.5/TEXTURE_SIZE));
	glm::vec3 p8(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE); // right top back
	glm::vec2 t8(0.5/TEXTURE_SIZE, 1-(0.5/TEXTURE_SIZE));



	int numCols = 16;
	int numRows = 16;

	double u = ((double)(ID % numCols) / (double)numCols);
	double v = ((double)(ID / numRows) / (double)numRows);
	double TILE_TEXTURE_SIZE = 15.99;

	glm::vec3 n1;


	if(activeStates[5] == false)
	{
		// Front Face Normal
		n1 = glm::vec3(0.0f, 0.0f, 1.0f);
		//Triangle 1
		vertexData.push_back(p1);
		uvData.push_back(glm::vec2(t1.x/TILE_TEXTURE_SIZE + u, t1.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p2);
		uvData.push_back(glm::vec2(t2.x/TILE_TEXTURE_SIZE + u, t2.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p3);
		uvData.push_back(glm::vec2(t3.x/TILE_TEXTURE_SIZE + u, t3.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
		//Triangle 2
		vertexData.push_back(p1);
		uvData.push_back(glm::vec2(t1.x/TILE_TEXTURE_SIZE + u, t1.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p3);
		uvData.push_back(glm::vec2(t3.x/TILE_TEXTURE_SIZE + u, t3.y/TILE_TEXTURE_SIZE + v ));
		normalData.push_back(n1);

		vertexData.push_back(p4);
		uvData.push_back(glm::vec2(t4.x/TILE_TEXTURE_SIZE + u, t4.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
	}

	if(activeStates[4] == false)
	{
		// Back
		n1 = glm::vec3(0.0f, 0.0f, -1.0f);
		//Triangle 1
		vertexData.push_back(p5);
		uvData.push_back(glm::vec2(t5.x/TILE_TEXTURE_SIZE + u, t5.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p6);
		uvData.push_back(glm::vec2(t6.x/TILE_TEXTURE_SIZE + u, t6.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p7);
		uvData.push_back(glm::vec2(t7.x/TILE_TEXTURE_SIZE + u, t7.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
		//Triangle 2
		vertexData.push_back(p5);
		uvData.push_back(glm::vec2(t5.x/TILE_TEXTURE_SIZE + u, t5.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p7);
		uvData.push_back(glm::vec2(t7.x/TILE_TEXTURE_SIZE + u, t7.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p8);
		uvData.push_back(glm::vec2(t8.x/TILE_TEXTURE_SIZE + u, t8.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
	}
	if(activeStates[1] == false)
	{
		// Right
		n1 = glm::vec3(1.0f, 0.0f, 0.0f);
		//Triangle 1
		vertexData.push_back(p2);
		uvData.push_back(glm::vec2(t2.x/TILE_TEXTURE_SIZE + u, t2.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p5);
		uvData.push_back(glm::vec2(t5.x/TILE_TEXTURE_SIZE + u, t5.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p8);
		uvData.push_back(glm::vec2(t8.x/TILE_TEXTURE_SIZE + u, t8.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
		//Triangle 2
		vertexData.push_back(p2);
		uvData.push_back(glm::vec2(t2.x/TILE_TEXTURE_SIZE + u, t2.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p8);
		uvData.push_back(glm::vec2(t8.x/TILE_TEXTURE_SIZE + u, t8.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p3);
		uvData.push_back(glm::vec2(t3.x/TILE_TEXTURE_SIZE + u, t3.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
	}
	if(activeStates[0] == false)
	{
		// left
		n1 = glm::vec3(-1.0f, 0.0f, 0.0f);
		//Triangle 1
		vertexData.push_back(p6);
		uvData.push_back(glm::vec2(t6.x/TILE_TEXTURE_SIZE + u, t6.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p1);
		uvData.push_back(glm::vec2(t1.x/TILE_TEXTURE_SIZE + u, t1.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p4);
		uvData.push_back(glm::vec2(t4.x/TILE_TEXTURE_SIZE + u, t4.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
		//Triangle 2
		vertexData.push_back(p6);
		uvData.push_back(glm::vec2(t6.x/TILE_TEXTURE_SIZE + u, t6.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p4);
		uvData.push_back(glm::vec2(t4.x/TILE_TEXTURE_SIZE + u, t4.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p7);
		uvData.push_back(glm::vec2(t7.x/TILE_TEXTURE_SIZE + u, t7.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
	}
	if(activeStates[3] == false)
	{
		// Top
		n1 = glm::vec3(0.0f, 1.0f, 0.0f);
		//Triangle 1
		vertexData.push_back(p4);
		uvData.push_back(glm::vec2(t6.x/TILE_TEXTURE_SIZE + u, t6.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p3);
		uvData.push_back(glm::vec2(t1.x/TILE_TEXTURE_SIZE + u, t1.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p8);
		uvData.push_back(glm::vec2(t4.x/TILE_TEXTURE_SIZE + u, t4.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
		//Triangle 2
		vertexData.push_back(p4);
		uvData.push_back(glm::vec2(t6.x/TILE_TEXTURE_SIZE + u, t6.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p8);
		uvData.push_back(glm::vec2(t4.x/TILE_TEXTURE_SIZE + u, t4.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p7);
		uvData.push_back(glm::vec2(t7.x/TILE_TEXTURE_SIZE + u, t7.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
	}

	if(activeStates[2] == false)
	{
		// Bottom
		n1 = glm::vec3(0.0f, -1.0f, 0.0f);

		//Triangle 1
		vertexData.push_back(p6);
		uvData.push_back(glm::vec2(t6.x/TILE_TEXTURE_SIZE + u, t6.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p5);
		uvData.push_back(glm::vec2(t1.x/TILE_TEXTURE_SIZE + u, t1.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p2);
		uvData.push_back(glm::vec2(t4.x/TILE_TEXTURE_SIZE + u, t4.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
		//Triangle 2
		vertexData.push_back(p6);
		uvData.push_back(glm::vec2(t6.x/TILE_TEXTURE_SIZE + u, t6.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p2);
		uvData.push_back(glm::vec2(t4.x/TILE_TEXTURE_SIZE + u, t4.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);

		vertexData.push_back(p1);
		uvData.push_back(glm::vec2(t7.x/TILE_TEXTURE_SIZE + u, t7.y/TILE_TEXTURE_SIZE + v));
		normalData.push_back(n1);
	}

	/*glm::vec2 t1(0.5/256, 0.5/256);

	glm::vec2 t2(1-(0.5/256), 0.5/256);

	glm::vec2 t3(1-(0.5/256), 1-(0.5/256));

	glm::vec2 t4(0.5/256, 1-(0.5/256));

	glm::vec2 t5(0.5/256, 0.5/256);

	glm::vec2 t6(1-(0.5/256), 0.5/256);

	glm::vec2 t7(1-(0.5/256), 1-(0.5/256));

	glm::vec2 t8(0.5/256, 1-(0.5/256));
	*/

	/*
	for(int i = 0; i < vertexData.size(); i+=3)
	{

	// get the three vertices that make the faces
	glm::vec3 p1 = vertexData[i+0];
	glm::vec3 p2 = vertexData[i+1];
	glm::vec3 p3 = vertexData[i+2];

	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 normal = glm::cross( v1,v2 );

	normal = glm::normalize(normal);

	normalData[i+0] = normal;
	normalData[i+1] = normal;
	normalData[i+2] = normal;

	}
	*/
}

void Chunk::Render()
{


	MoveRotateScale(chunkPosition, glm::vec3(0,0,0), glm::vec3(1,1,1));
	glUniformMatrix4fv(engine->getGraphicsManager()->getMatrixID(), 1, GL_FALSE, &chunkMVP[0][0]);

	//vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  
		3,                  
		GL_FLOAT,           
		GL_FALSE,           
		0,                  
		(void*)0           
		);

	//UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() );

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//glBindTexture(GL_TEXTURE_2D, 0); 
}