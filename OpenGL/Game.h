#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>
#include <iostream>

#include "Graphics3D.h"
#include "Input.h"


class Game
{
public:
	Game(void);
	void Init();
	void Update();

	Graphics3D graphics;
	Input input;

	enum GameState { ShowingSplash, Paused, 
          ShowingMenu, Playing, Exiting };
  
	GameState gameState;
private:

};

