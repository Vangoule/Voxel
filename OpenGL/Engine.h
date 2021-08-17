#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>

#include "Window.h"
#include "Input.h"
#include "Graphics3D.h"
#include "Game.h"





class Engine {
	

public:
	Engine();
	void init();

	Window* getWindow(){
		return window;
	}

	sf::Window* getWindowHandle(){
		return window->getHandle();
	}
	Game* getGame(){
		return game;
	}
	Graphics3D* getGraphicsManager(){
		return graphicsManager;
	}
	Input* getInputManager(){
		return inputManager;
	}

private:
	Window* window;
	Game* game;
	Input* inputManager;
	Graphics3D* graphicsManager;


};




