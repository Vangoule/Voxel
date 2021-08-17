#include "Input.h"
#include "Globals.h"

#include <fstream>
#include <sstream>

Input::Input(void)
{
}

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 Input::getViewMatrix(){
        return ViewMatrix;
}
glm::mat4 Input::getProjectionMatrix(){
        return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 126, -15 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 0.0f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 6.0f; // 3 units / second
float mouseSpeed = 0.05f;

float deltaTime = 0.033;
bool mouseActive = true;
glm::vec3 right;
glm::vec3 up;
glm::vec3 direction;

void Input::HandleInput()
{
	engine->getWindow()->GetFrameRate();
	//deltaTime = engine->getWindow()->getDelta();
//	std::cout << deltaTime << std::endl;
	if(mouseActive)
	{
					        // Get mouse position
				int xpos, ypos;
				xpos = engine->getWindow()->getMousePos().x;
				ypos = engine->getWindow()->getMousePos().y;

				int halfx = engine->getWindowHandle()->getSize().x/2;
				int halfy = engine->getWindowHandle()->getSize().y/2;
				// Reset mouse position for next frame
				engine->getWindow()->setMousePos(halfx, halfy);
				// Compute new orientation
				horizontalAngle += mouseSpeed * deltaTime  * float(halfx - xpos );
				verticalAngle   += mouseSpeed * deltaTime  * float(halfy - ypos );
        // Direction : Spherical coordinates to Cartesian coordinates conversion
        direction = glm::vec3(
                cos(verticalAngle) * sin(horizontalAngle), 
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
        );
        
        // Right vector
       right = glm::vec3(
                sin(horizontalAngle - 3.14f/2.0f), 
                0,
                cos(horizontalAngle - 3.14f/2.0f)
        );
        
        // Up vector
       up = glm::cross( right, direction );

	}

	 sf::Event event;
		while (engine->getWindowHandle()->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
			{
                engine->getWindowHandle()->close();
				exit(0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				engine->getWindowHandle()->close();
				exit(0);
			}
			if (event.type == sf::Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);
			}
			if( sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash))
			{
				mouseActive = false;
			}
			if( sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			{
				mouseActive = true;
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				
			}

			if(event.type == sf::Event::MouseMoved)
			{

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				engine->getGraphicsManager()->load = true;
				
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				position += direction * deltaTime * speed;
				
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				 position -= right *  deltaTime * speed;
				
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
			
				position += right * deltaTime * speed;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
			
				position -= direction * deltaTime * speed;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				speed = 20.0f;
			}else
			{
				speed = 10.0f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
			{
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
			{
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			}
		float FoV = initialFoV;
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 10000.0f);
        // Camera matrix
        ViewMatrix       = glm::lookAt(
                                                                position,           // Camera is here
                                                                position+direction, // and looks here : at the same position, plus "direction"
                                                                up                  // Head is up (set to 0,-1,0 to look upside-down)
                                                   );
        }
}
