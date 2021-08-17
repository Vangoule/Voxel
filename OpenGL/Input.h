#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Input
{
public:
	Input(void);
	void HandleInput();
	glm::mat4 getViewMatrix();

	glm::mat4 getProjectionMatrix();
};

