#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>

#include <vector>


class Model
{
public:
	Model();
	void Setup(std::string fName, std::string tName);
	void Render();
	void MoveRotateScale(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	void SetMatrix(float bulletTransform[16]);
	void Rotate(glm::vec3 rot);
	void Scale(glm::vec3 scale);
	void Move(glm::vec3 pos);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	glm::mat4 modelMVP;

	GLuint Texture;

	GLuint vertexbuffer;
	GLuint uvbuffer;

	std::string texture;
	std::string fileName;
};

