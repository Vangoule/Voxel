#include "ChunkManager.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
ChunkManager::ChunkManager(void)
{

}


ChunkManager::~ChunkManager(void)
{
}

void ChunkManager::Update()
{	
	/*
	for(std::vector<Chunk*>::iterator it = chunkList.begin(); it != chunkList.end(); ++it) 
	{
	int x = rand() % 16 ;
	int y = rand() % 256 ;
	int z = rand() % 16 ;
	chunkList[0]->worldBlocks[x][y][z] = 0;
	chunkList[0]->RebuildMesh();
	}
	*/


}

bool ChunkManager::LoadChunk(int x, int y, Chunk* chunkToLoad)
{
	chunkToLoad->Load();
	int worldBlocks[16][256][16];
	std::string fileName;
	std::stringstream out;
	out << "Chunk";
	out << chunkToLoad->chunkPosition.x;
	out << "_";
	out << chunkToLoad->chunkPosition.y;
	out << "_";
	out << chunkToLoad->chunkPosition.z;
	out << ".chunk";
	fileName = out.str();
	std::ifstream in(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if(in.is_open())
	{
		int size;
		size = (int) in.tellg();
		in.seekg (0, std::ios::beg);
		in.read((char *) &worldBlocks, size);
		for (int x = 0; x < 16; x++)
		{
			for (int y = 0; y < 256; y++)
			{
				for (int z = 0; z < 16; z++)
				{
					chunkToLoad->worldBlocks[x][y][z] = worldBlocks[x][y][z];
				}
			}
		}
		in.close();
		return true;
	}else
	{
		return false;
	}
	return false;
}



void ChunkManager::SaveChunk(int x, int y)
{

	for(std::vector<Chunk*>::iterator it = chunkList.begin(); it != chunkList.end(); ++it) 
	{
		std::string fileName;
		std::stringstream out;
		out << "Chunk";
		out << (*it)->chunkPosition.x;
		out << "_";
		out << (*it)->chunkPosition.y;
		out << "_";
		out << (*it)->chunkPosition.z;
		out << ".chunk";
		fileName = out.str();
		std::cout << fileName << " - Saved" << std::endl;
		std::ofstream outbin( fileName, std::ios::out | std::ios::binary);
		outbin.write( reinterpret_cast <const char*> ((*it)->worldBlocks), sizeof((*it)->worldBlocks) );
		outbin.close();
	}

}


void ChunkManager::CreateChunk(int x, int y)
{



	/*for(int i = 0; i < 1; i++)
	{
	for(int j = 0; j < 1; j++)
	{
	Chunk* chunk = new Chunk();

	chunk->chunkPosition = glm::vec3(i * (chunk->CHUNK_SIZE_X), 0 , j * (chunk->CHUNK_SIZE_Z));

	chunk->Init();

	chunk->CreateMesh();

	chunkList.push_back(chunk);
	}
	}
	*///std::cout << "THIS THREAD = " << std::this_thread::get_id() << std::endl;

	bool chunkExists = false;

	for(std::vector<Chunk*>::iterator it = chunkList.begin(); it != chunkList.end(); ++it) 
	{
		if((*it)->chunkPosition == glm::vec3(x * ((*it)->CHUNK_SIZE_X), 0 , y * ((*it)->CHUNK_SIZE_Z)))
		{
			chunkExists = true;
		}
	}

	if(chunkExists == false)
	{
		Chunk* chunk = new Chunk();

		chunk->chunkPosition = glm::vec3(x * (chunk->CHUNK_SIZE_X), 0 , y * (chunk->CHUNK_SIZE_Z));

		//		

		if(LoadChunk(x,y, chunk) == false)
		{

			chunk->Init();
			std::cout << "Creating New Chunk at: " << x << " " << y << std::endl;
		}else{
			std::cout << "Loading Chunk at: " << x << " " << y << std::endl;
		}


		chunk->CreateMesh();

		SaveChunk(x,y);

		chunkList.push_back(chunk);
	}

}

void ChunkManager::RenderChunks()
{
	for(std::vector<Chunk*>::iterator it = chunkList.begin(); it != chunkList.end(); ++it) 
	{

		(*it)->Render();

	}

}