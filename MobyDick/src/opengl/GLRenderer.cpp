#include "GLRenderer.h"


#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "../Globals.h"
#include "../GameConfig.h"
#include "GLDrawer.h"
#include "SpriteVertex.h"




GLRenderer::GLRenderer()
{

	//Create the different drawers
	m_spriteDrawer = std::make_shared<GLDrawer>(GameObjectType::SPRITE);
	m_lineDrawer = std::make_shared<GLDrawer>(GameObjectType::LINE);

	//Initilaize the various draw batches that can exist
	for (int objectTypeIndex = 1; objectTypeIndex < MAX_OBJECT_TYPES; objectTypeIndex++) {
		for (int textureIndex = 0; textureIndex < MAX_GL_TEXTURES; textureIndex++) {
			for (int shaderIndex = 0; shaderIndex < MAX_GL_SHADER_TYPES; shaderIndex++) {

				m_drawBatches[objectTypeIndex][textureIndex][shaderIndex] = { DrawBatch(1) };

			}
		}
	}

	

}


void GLRenderer::init(SDL_Window* window)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	SDL_GLContext m_glcontext = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	m_projectionMatrix = glm::ortho(0.0f, (float)GameConfig::instance().windowWidth(), (float)GameConfig::instance().windowHeight(), 0.0f, 0.0f, 10.0f);





}

void GLRenderer::drawSprite(SDL_FRect quad, SDL_Color color, SDL_Texture* texture, SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor)
{

	glm::vec2 glPosition{quad.x, quad.y};
	glm::vec2 glSize{ quad.w, quad.h };

	glm::vec4 glColor{ color.r, color.g, color.b, color.a};

	//Array of 4 vertices
	SpriteVertex spriteVertexBuffer[4];

	//Initilaize a new translation matrix with one to start it out as an identity matrix
	glm::mat4 translationMatrix(1.0f);

	//Apply the position to the translation matrix
	translationMatrix = glm::translate(translationMatrix, glm::vec3(glPosition.x, glPosition.y, 1.0));

	//Apply the rotation - move to center, rotate, move back
	translationMatrix = glm::translate(translationMatrix, glm::vec3(glSize.x / 2, glSize.y / 2, 0.0));
	translationMatrix = glm::rotate(translationMatrix, angle, glm::vec3(0.0, 0.0, 1.0));
	translationMatrix = glm::translate(translationMatrix, glm::vec3(-(glSize.x / 2), -(glSize.y / 2), 0.0));

	//
	// Vertex Buffer Data
	// 	   Build the 4 vertices that make a quad/rectangle/square
	// 

	//v0
	spriteVertexBuffer[0].positionAttribute = glm::vec3(0, 0, -1);
	//spriteVertexBuffer[0].positionAttribute = position;
	spriteVertexBuffer[0].colorAttribute = glColor;
	spriteVertexBuffer[0].textureCoordsAttribute = { textureSrcQuad->x, textureSrcQuad->y };

	//v1
	spriteVertexBuffer[1].positionAttribute = glm::vec3{ glSize.x, 0, -1 };
	//spriteVertexBuffer[1].positionAttribute = position + glm::vec3{ width, 0, 0 };
	spriteVertexBuffer[1].colorAttribute = glColor;
	spriteVertexBuffer[1].textureCoordsAttribute = { 1,0 };

	//v2
	spriteVertexBuffer[2].positionAttribute = glm::vec3{ glSize.x, glSize.y, -1 };
	//spriteVertexBuffer[2].positionAttribute = position + glm::vec3{ width, height, 0 };
	spriteVertexBuffer[2].colorAttribute = glColor;
	spriteVertexBuffer[2].textureCoordsAttribute = { 1,1 };

	//v3
	spriteVertexBuffer[3].positionAttribute = glm::vec3{ 0, glSize.y, -1 };
	//spriteVertexBuffer[3].positionAttribute = glm::vec3{ 0, height, 0 };
	spriteVertexBuffer[3].colorAttribute = glColor;
	spriteVertexBuffer[3].textureCoordsAttribute = { 0,1 };

	//Apply the tranlation matrix to each vertex
	for (int i = 0; i < 4; i++) {

		spriteVertexBuffer[i].positionAttribute = translationMatrix * glm::vec4(spriteVertexBuffer[i].positionAttribute, 1.0);

	}

	//m_drawBatches[GameObjectType::SPRITE][][]

}


void GLRenderer::drawSprite(int objectType, glm::vec2 position, GLint zLayer, GLfloat angle, GLfloat width, GLfloat height, glm::vec4 color, GLuint textureId, glm::vec2 textureCoords)
{

	glBindTexture(GL_TEXTURE_2D, textureId);

	SpriteVertex spriteVertexBuffer[4];


	float angleRad = util::degreesToRadians(angle);

	//Initilaize a new translation matrix with one to start it out as an identity matrix
	glm::mat4 translationMatrix(1.0f);

	//Apply the position to the translation matrix
	translationMatrix = glm::translate(translationMatrix, glm::vec3(position.x, position.y, 1.0));

	//Apply the rotation - move to center, rotate, move back
	translationMatrix = glm::translate(translationMatrix, glm::vec3(width / 2, height / 2, 0.0));
	translationMatrix = glm::rotate(translationMatrix, angleRad, glm::vec3(0.0, 0.0, 1.0));
	translationMatrix = glm::translate(translationMatrix, glm::vec3(-(width / 2), -(height / 2), 0.0));

	//
	// Vertex Buffer Data
	// 	   Build the 4 vertices that make a quad/rectangle/square
	// 
	
	//v0
	spriteVertexBuffer[0].positionAttribute = glm::vec3(0,0, zLayer);
	//spriteVertexBuffer[0].positionAttribute = position;
	spriteVertexBuffer[0].colorAttribute = color;
	spriteVertexBuffer[0].textureCoordsAttribute = textureCoords;

	//v1
	spriteVertexBuffer[1].positionAttribute = glm::vec3{ width, 0, zLayer };
	//spriteVertexBuffer[1].positionAttribute = position + glm::vec3{ width, 0, 0 };
	spriteVertexBuffer[1].colorAttribute = color;
	spriteVertexBuffer[1].textureCoordsAttribute = {1,0};

	//v2
	spriteVertexBuffer[2].positionAttribute = glm::vec3{ width, height, zLayer };
	//spriteVertexBuffer[2].positionAttribute = position + glm::vec3{ width, height, 0 };
	spriteVertexBuffer[2].colorAttribute = color;
	spriteVertexBuffer[2].textureCoordsAttribute = { 1,1 };

	//v3
	spriteVertexBuffer[3].positionAttribute = glm::vec3{ 0, height, zLayer };
	//spriteVertexBuffer[3].positionAttribute = glm::vec3{ 0, height, 0 };
	spriteVertexBuffer[3].colorAttribute = color;
	spriteVertexBuffer[3].textureCoordsAttribute = { 0,1 };

	//Apply the tranlation matrix to each vertex
	for (int i = 0; i < 4; i++) {

		spriteVertexBuffer[i].positionAttribute = translationMatrix * glm::vec4(spriteVertexBuffer[i].positionAttribute, 1.0);

	}

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(spriteVertexBuffer), &spriteVertexBuffer);

	//
	// Index Buffer Data
	// 
	//SpriteindexBuffer spriteIndexBuffer;
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer), g_index_buffer, GL_DYNAMIC_DRAW); // state using
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spriteIndexBuffer), &spriteIndexBuffer, GL_DYNAMIC_DRAW);


}

void GLRenderer::bind()
{
	//glBindVertexArray(m_vao);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

GLRenderer::~GLRenderer()
{

	//glDeleteTextures(1, texture_id);

	//glDeleteVertexArrays(1, &m_vao);

    SDL_GL_DeleteContext(m_glcontext);

}

/*
For each type of object we want to render (sprite, line, pixel, rectangle), 
set the vertextArrtribute layout/structure. This should only have to done once for each type
and is stored using the VAO - vertex array object.
When doing individual draws, you will bind to the appropriate VAO before adding the vertex and index
buffer data
*/
void GLRenderer::_setVertexBufferAttriubuteLayout()
{

	// vertex attrubute indexes
	const int attrib_position = 0;
	const int attrib_color = 1;
	const int attrib_texture = 2;


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
	glVertexAttribPointer(attrib_texture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(7 * sizeof(float)));



}

