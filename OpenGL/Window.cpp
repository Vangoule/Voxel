#include "Window.h"
#include <iostream>
#include <sstream>
Window::Window(void)
{
}


void Window::Init()
{

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 2;

	windowHndl.create(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Resize, settings);
	windowHndl.setPosition(sf::Vector2i(32,0));
	sf::ContextSettings settingsCheck = windowHndl.getSettings();
	std::cout << "Initiating Window" << std::endl;
	std::cout << "Your computer supports: " << std::endl;
	std::cout << "Depth Bits:" << settingsCheck.depthBits << std::endl;
	std::cout << "Stencil Bits:" << settingsCheck.stencilBits << std::endl;
	std::cout << "Antialiasing Level:" << settingsCheck.antialiasingLevel << std::endl;
	std::cout << "Version:" << settingsCheck.majorVersion << "." << settingsCheck.minorVersion << std::endl << std::endl;

	windowHndl.setVerticalSyncEnabled(false);
	windowHndl.setKeyRepeatEnabled(false);
	windowHndl.setFramerateLimit(120);
	


}

sf::Vector2i Window::getMousePos()
{
	sf::Vector2i localPosition = sf::Mouse::getPosition(windowHndl); // window is a sf::Window
	return localPosition;
}

void Window::setMousePos(int posx, int posy)
{
		sf::Mouse::setPosition(sf::Vector2i(posx, posy), windowHndl);
}

unsigned int Window::GetFrameRate(void)
	{
		float Framerate = 1.f / Clock.getElapsedTime().asSeconds();
		Clock.restart();
		
		std::stringstream ss;
		ss << Framerate;
		std::string title = ss.str();
		windowHndl.setTitle(title);
		return Framerate;
	}
float oldTimeSinceStart = 0;
float Window::getDelta()
{
	

	float timeSinceStart = deltaClock.getElapsedTime().asSeconds();
    float deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
	return deltaTime;
}