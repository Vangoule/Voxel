#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>

class Window
{
public:
	Window(void);
	void Init();

	sf::Window* getHandle(){
		return &windowHndl;
	}
	
	sf::Vector2i getMousePos();
	void setMousePos(int posx, int posy);
	unsigned int GetFrameRate();
	sf::Clock Clock;
	sf::Clock deltaClock;
	float getDelta();
private:
	sf::Window windowHndl;
};

