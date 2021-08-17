#pragma once

class NoiseManager
{
public:
	NoiseManager(void);
	~NoiseManager(void);
	void CreateHeightmap();

	float getHeightAt(int x, int y);
private:

};

