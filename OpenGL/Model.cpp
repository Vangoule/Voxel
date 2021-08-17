#include "Model.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include <GL/GLU.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objloader.hpp"

#include "globals.h"



Model::Model()
{
	
}




void Model::Setup(std::string fName, std::string tName)
{
	fileName = fName;
	texture = tName;
	bool res = loadOBJ(fileName.c_str(), vertices, uvs, normals);

/*	for(int i = 0; i < uvs.size(); i++)
	{
		uvs[i]*=1;
	}
*/	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	Texture = engine->getGraphicsManager()->loadTexture(texture.c_str());

	modelMVP = engine->getGraphicsManager()->getMVP();

}

void Model::MoveRotateScale(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	
	
	glm::mat4 T = glm::translate(glm::mat4(1.0f),pos);
	glm::mat4 Rx = glm::rotate(T, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry = glm::rotate(Rx, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 AlmostModel = glm::rotate(Ry, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 Model = glm::scale(AlmostModel, scale);

	modelMVP        = engine->getGraphicsManager()->getProjection() * engine->getGraphicsManager()->getView() * Model;


}

void Model::SetMatrix(float bulletTransform[16])
{

	glm::mat4 Model = glm::make_mat4(bulletTransform);
	modelMVP        = engine->getGraphicsManager()->getProjection() * engine->getGraphicsManager()->getView() * Model;

}

void Model::Rotate(glm::vec3 rot)
{
	modelMVP = glm::rotate(modelMVP, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMVP = glm::rotate(modelMVP, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMVP = glm::rotate(modelMVP, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::Scale(glm::vec3 scale)
{
	modelMVP = glm::scale(modelMVP, scale);
	
}

void Model::Move(glm::vec3 pos)
{
	modelMVP = glm::translate(modelMVP,pos);
}

void Model::Render()
{


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture); 
	glUniform1i(engine->getGraphicsManager()->getTextureID(), 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glUniformMatrix4fv(engine->getGraphicsManager()->getMatrixID(), 1, GL_FALSE, &modelMVP[0][0]);

		//vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
	   0,                  
	   3,                  
	   GL_FLOAT,           
	   GL_FALSE,           
	   0,                  
	   (void*)0           
	);

     //UVs
     glEnableVertexAttribArray(1);
	 glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
     glVertexAttribPointer(
       1,                                // attribute
	   2,                                // size
       GL_FLOAT,                         // type
       GL_FALSE,                         // normalized?
       0,                                // stride
       (void*)0                          // array buffer offset
     );
 
	
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
    

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

	glBindTexture(GL_TEXTURE_2D, 0); 

}
