#include "GL2Renderer.h"


#include <iostream>

#include "opengl/GLDebugCallback.h"
#include "Globals.h"




GL2Renderer& GL2Renderer::instance()
{

    static GL2Renderer singletonInstance;
    return singletonInstance;

}


void GL2Renderer::init(SDL_Window* window)
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

	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	//ToDo: need function to build the GLDrawers, aeach shoul dhave its own VAO and VBO ID stored with it
	_buildDrawers();

	//Generate 1 vao
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//Generate 1 vbo
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//	set the vertextArrtribute layout / structure
	_setVertexBufferAttriubuteLayout();

	//Generate 1 ibo
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//unbind vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GL2Renderer::drawSprite(int objectType, glm::vec2 position, GLint zLayer, GLfloat angle, GLfloat width, GLfloat height, glm::vec4 color, GLuint textureId, glm::vec2 textureCoords)
{

	//glBindTexture(GL_TEXTURE_2D, textureId);

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
	
	glm::vec2 textureCoordNormalized{};

	//v0
	spriteVertexBuffer[0].positionAttribute = glm::vec3(0,0, zLayer);
	spriteVertexBuffer[0].colorAttribute = color;
	textureCoordNormalized = { (textureCoords.x + .5) / 4096, (textureCoords.y + .5) / 4096 };
	spriteVertexBuffer[0].textureCoordsAttribute = { textureCoordNormalized.x,textureCoordNormalized.y};


	//v1
	spriteVertexBuffer[1].positionAttribute = glm::vec3{ width, 0, zLayer };
	spriteVertexBuffer[1].colorAttribute = color;
	textureCoordNormalized = { (textureCoords.x + width + .5) / 4096, (textureCoords.y + .5) / 4096 };
	spriteVertexBuffer[1].textureCoordsAttribute = { textureCoordNormalized.x,textureCoordNormalized.y };

	//v2
	spriteVertexBuffer[2].positionAttribute = glm::vec3{ width, height, zLayer };
	spriteVertexBuffer[2].colorAttribute = color;
	textureCoordNormalized = { (textureCoords.x + width + .5) / 4096, (textureCoords.y + height + .5) / 4096 };
	spriteVertexBuffer[2].textureCoordsAttribute = { textureCoordNormalized.x,textureCoordNormalized.y };


	//v3
	spriteVertexBuffer[3].positionAttribute = glm::vec3{ 0, height, zLayer };
	spriteVertexBuffer[3].colorAttribute = color;
	textureCoordNormalized = { (textureCoords.x + .5) / 4096, (textureCoords.y + height + .5) / 4096 };
	spriteVertexBuffer[3].textureCoordsAttribute = { textureCoordNormalized.x,textureCoordNormalized.y };


	//Apply the tranlation matrix to each vertex
	for (int i = 0; i < 4; i++) {

		spriteVertexBuffer[i].positionAttribute = translationMatrix * glm::vec4(spriteVertexBuffer[i].positionAttribute, 1.0);

	}

	auto test = sizeof(spriteVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(spriteVertexBuffer), &spriteVertexBuffer);

	//
	// Index Buffer Data
	// 
	SpriteindexBuffer spriteIndexBuffer;
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer), g_index_buffer, GL_DYNAMIC_DRAW); // state using
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spriteIndexBuffer), &spriteIndexBuffer, GL_DYNAMIC_DRAW);


}

void GL2Renderer::bind()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

GL2Renderer::~GL2Renderer()
{

	//glDeleteTextures(1, texture_id);

	glDeleteVertexArrays(1, &m_vao);

    SDL_GL_DeleteContext(m_glcontext);

}

/*
For each type of object we want to render (sprite, line, pixel, rectangle), 
set the vertextArrtribute layout/structure. This should only have to done once for each type
and is stored using the VAO - vertex array object.
When doing individual draws, you will bind to the appropriate VAO before adding the vertex and index
buffer data
*/
void GL2Renderer::_setVertexBufferAttriubuteLayout()
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

void GL2Renderer::_buildDrawers()
{

	//Build a sprite drawer
	//m_glDrawers[GLDrawerType::GLSPRITE] = GLDrawer(GLDrawerType::GLSPRITE);
		


}

