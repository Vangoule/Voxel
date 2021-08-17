#include "Engine.h"


Engine::Engine(void)
{

}


void Engine::init()
{
	std::cout << "Initiating Engine" << std::endl << std::endl;

	window = new Window();
	window->Init();

	game = new Game();
	game->Init();

	graphicsManager = new Graphics3D();
	
	std::cout << "Initiating Input" << std::endl << std::endl;
	inputManager = new Input();

}

