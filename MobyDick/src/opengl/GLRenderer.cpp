#include "GLRenderer.h"


#include <iostream>
#include <sstream>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <format>


#include "../Globals.h"
#include "../GameConfig.h"
#include "GLDrawer.h"
#include "SpriteVertex.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

GLRenderer::GLRenderer()
{


}


void GLRenderer::init(SDL_Window* window)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);


	SDL_GLContext m_glcontext = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	m_projectionMatrix = glm::ortho(0.0f, (float)GameConfig::instance().windowWidth(), (float)GameConfig::instance().windowHeight(), 0.0f, 0.0f, 10.0f);

	//Create the different drawers
	m_spriteDrawer = std::make_shared<GLDrawer>(GLDrawerType::GLSPRITE);
	m_lineDrawer = std::make_shared<GLDrawer>(GLDrawerType::GLLINE);

	//Create all shaders
	m_shaders[(int)GLShaderType::BASIC] = Shader(GLShaderType::BASIC);
	m_shaders[(int)GLShaderType::UBER] = Shader(GLShaderType::UBER);

	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	



}

bool GLRenderer::clear()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

bool GLRenderer::present()
{

	//Draw each batch
	for (auto& drawBatch : m_drawBatches) {
		drawBatch.second.draw();
		drawBatch.second.clear();
	}

	SDL_GL_SwapWindow(game->window());
	SDL_Delay(1);

	return true;
}

void GLRenderer::drawSprite(SDL_FRect quad, SDL_Color color, Texture* texture, SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor)
{


	//Temporary - we need to normalize the color coming in
	color = { 1,1,1,1 };

	glm::vec2 glPosition{quad.x, quad.y};
	glm::vec2 glSize{ quad.w, quad.h };
	glm::vec4 glColor{ color.r, color.g, color.b, color.a};
	//std::shared_ptr<SpriteVertex> vertex;

	//Array of 4 vertices
	std::vector<SpriteVertex> spriteVertexBuffer;

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

	SpriteVertex vertex;
	//v0
	vertex.positionAttribute = glm::vec3(0, 0, -1);
	vertex.colorAttribute = glColor;
	glm::vec2 calculatedTextureCoordinates = { textureSrcQuad->x, textureSrcQuad->y };
	auto normalizedTextureCoords = util::normalizeTextureCoords(
		{ calculatedTextureCoordinates.x, calculatedTextureCoordinates.y }, 
		{ texture->surface->w, texture->surface->h });
	vertex.textureCoordsAttribute = normalizedTextureCoords;
	spriteVertexBuffer.push_back(vertex);

	//v1
	vertex.positionAttribute = glm::vec3{ glSize.x, 0, -1 };
	vertex.colorAttribute = glColor;

	calculatedTextureCoordinates = { textureSrcQuad->x + textureSrcQuad->w, textureSrcQuad->y };
	normalizedTextureCoords = util::normalizeTextureCoords(
		{ calculatedTextureCoordinates.x, calculatedTextureCoordinates.y },
		{ texture->surface->w, texture->surface->h });
	vertex.textureCoordsAttribute = normalizedTextureCoords;

	spriteVertexBuffer.push_back(vertex);

	//v2
	vertex.positionAttribute = glm::vec3{ glSize.x, glSize.y, -1 };
	vertex.colorAttribute = glColor;

	calculatedTextureCoordinates = { textureSrcQuad->x + textureSrcQuad->w, textureSrcQuad->y + textureSrcQuad->h };
	normalizedTextureCoords = util::normalizeTextureCoords(
		{ calculatedTextureCoordinates.x, calculatedTextureCoordinates.y },
		{ texture->surface->w, texture->surface->h });
	vertex.textureCoordsAttribute = normalizedTextureCoords;

	spriteVertexBuffer.push_back(vertex);

	//v3
	vertex.positionAttribute = glm::vec3{ 0, glSize.y, -1 };
	vertex.colorAttribute = glColor;

	calculatedTextureCoordinates = { textureSrcQuad->x , textureSrcQuad->y + textureSrcQuad->h };
	normalizedTextureCoords = util::normalizeTextureCoords(
		{ calculatedTextureCoordinates.x, calculatedTextureCoordinates.y },
		{ texture->surface->w, texture->surface->h });
	vertex.textureCoordsAttribute = normalizedTextureCoords;

	spriteVertexBuffer.push_back(vertex);

	//Apply the tranlation matrix to each vertex
	for (int i = 0; i < 4; i++) {

		spriteVertexBuffer[i].positionAttribute = translationMatrix * glm::vec4(spriteVertexBuffer[i].positionAttribute, 1.0);

	}

	//shader needs to be passed in
	auto shadertype = GLShaderType::BASIC;

	_addVertexBuffer(spriteVertexBuffer, GLDrawerType::GLSPRITE, texture, shadertype);

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

void GLRenderer::_addVertexBuffer(std::vector<SpriteVertex> spriteVertices, GLDrawerType objectType, Texture* texture, GLShaderType shaderType)
{
	//Build the map key
	//const void* texturePtr = static_cast<const void*>(texture);
	std::stringstream ss;
	ss << texture;
	auto key = std::format("{:0d}_{}_{:05d}", (int)objectType, ss.str(), (int)shaderType);
	

	//See if the drawBatch for this combo exists yet
	if (m_drawBatches.find(key) == m_drawBatches.end()) {

		m_drawBatches[key] = DrawBatch(objectType, texture, shaderType);
		m_drawBatches[key].addVertexBuffer(spriteVertices);

	}
	else {

		m_drawBatches[key].addVertexBuffer(spriteVertices);

	}


}

void GLRenderer::_addVertexBuffer(std::vector<std::shared_ptr<Vertex>> vertex)
{






}
