#include "Game.h"
#include "Globals.h"

#include <fstream>
#include <iostream>
#include <sstream>


Game::Game(void)
{

}

void Game::Init()
{
	std::cout << "Initiating Game" << std::endl << std::endl;

	engine->getGraphicsManager()->Init();

	gameState = Playing;		

}

void Game::Update()
{
	

	switch(gameState)
	{
		default: break;
		case ShowingSplash: break;
		case Playing: 
			engine->getInputManager()->HandleInput();
			engine->getGraphicsManager()->Draw();
			break;
	}
	
}
