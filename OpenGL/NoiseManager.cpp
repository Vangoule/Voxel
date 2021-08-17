#include "NoiseManager.h"
#include <noise\noise.h>
#include "noiseutils.h"

NoiseManager::NoiseManager(void)
{
}

NoiseManager::~NoiseManager(void)
{
}
utils::NoiseMap heightMap;
void NoiseManager::CreateHeightmap()
{
	module::Perlin myModule;
	myModule.SetOctaveCount (2);
	myModule.SetFrequency (1);
	myModule.SetPersistence (0.25);
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule (myModule);
	heightMapBuilder.SetDestNoiseMap (heightMap);
	heightMapBuilder.SetDestSize (2048, 2048);
    heightMapBuilder.SetBounds (0, 10.0, 0, 10.0);
	heightMapBuilder.Build ();
}

float NoiseManager::getHeightAt(int x, int y)
{
	return heightMap.GetValue(x,y);
}
