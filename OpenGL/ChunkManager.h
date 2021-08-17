#pragma once

#include <iostream>
#include <vector>
#include "Chunk.h"

class ChunkManager
{
public:
	ChunkManager(void);

	~ChunkManager(void);

	void CreateChunk(int x, int y);

	void SaveChunk(int x, int y);

	bool LoadChunk(int x, int y,Chunk* chunkToLoad);

	void RenderChunks();



	void Update();

	std::vector<Chunk*> chunkList;

};

