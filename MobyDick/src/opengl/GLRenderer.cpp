#include "GLRenderer.h"


#include <iostream>
#include <sstream>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <format>


#include "../Util.h"
#include "../GameConfig.h"
#include "GLDrawer.h"
#include "Vertex.h"
#include "LineDrawBatch.h"
#include "SpriteDrawBatch.h"
#include "DrawBatch.h"

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

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);


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
	m_spriteDrawer = GLDrawer(GLDrawerType::GLSPRITE);
	m_lineDrawer = GLDrawer(GLDrawerType::GLLINE);

	//Create all shaders
	m_shaders[(int)GLShaderType::BASIC] = Shader(GLShaderType::BASIC);
	m_shaders[(int)GLShaderType::UBER] = Shader(GLShaderType::UBER);
	m_shaders[(int)GLShaderType::LINE] = Shader(GLShaderType::LINE);

	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	
	//Generate the maximum number of possible texture Id's
	glGenTextures((int)GL_TextureIndexType::COUNT, m_textureIds);

}

bool GLRenderer::clear()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

bool GLRenderer::present()
{

	//Draw each batch if batching is turned on
	if (GameConfig::instance().openGLBatching() == true) {

		for (auto& drawBatch : m_drawBatches) {
			drawBatch.second->draw();
		}

		m_drawBatches.erase(m_drawBatches.begin(), m_drawBatches.end());

	}

	SDL_GL_SwapWindow(game->window());
	//SDL_Delay(1);

	return true;
}

void GLRenderer::drawSprite(SDL_FRect destQuad, SDL_Color color, Texture* texture, SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor)
{

	auto normalizedcolor = util::glNormalizeColor(color);


	glm::vec2 glPosition{ destQuad.x, destQuad.y};
	glm::vec2 glSize{ destQuad.w, destQuad.h };
	glm::vec4 glColor{ normalizedcolor.r, normalizedcolor.g, normalizedcolor.b, normalizedcolor.a};
	//std::shared_ptr<SpriteVertex> vertex;

	//Array of 4 vertices
	std::vector<SpriteVertex> spriteVertexBuffer;

	//Initilaize a new translation matrix with one to start it out as an identity matrix
	glm::mat4 translationMatrix(1.0f);

	//Apply the position to the translation matrix
	translationMatrix = glm::translate(translationMatrix, glm::vec3(glPosition.x, glPosition.y, 1.0));

	angle = util::degreesToRadians(angle);

	////Apply the rotation - move to center, rotate, move back
	translationMatrix = glm::translate(translationMatrix, glm::vec3(glSize.x / 2, glSize.y / 2, 0.0));
	translationMatrix = glm::rotate(translationMatrix, angle, glm::vec3(0.0, 0.0, 1.0));
	translationMatrix = glm::translate(translationMatrix, glm::vec3(-(glSize.x / 2), -(glSize.y / 2), 0.0));

	//
	// Vertex Buffer Data
	// 	   Build the 4 vertices that make a quad/rectangle/square
	// 
	int zIndex = -1;
	
	SpriteVertex vertex;
	//v0
	vertex.positionAttribute = glm::vec3(0, 0, zIndex);
	vertex.colorAttribute = glColor;
	glm::vec2 calculatedTextureCoordinates = { textureSrcQuad->x, textureSrcQuad->y };
	auto normalizedTextureCoords = util::glNormalizeTextureCoords(
		{ calculatedTextureCoordinates.x, calculatedTextureCoordinates.y }, 
		{ texture->surface->w, texture->surface->h });
	vertex.textureCoordsAttribute = normalizedTextureCoords;
	spriteVertexBuffer.push_back(vertex);

	//v1
	vertex.positionAttribute = glm::vec3{ glSize.x, 0, zIndex };
	vertex.colorAttribute = glColor;

	calculatedTextureCoordinates = { textureSrcQuad->x + textureSrcQuad->w, textureSrcQuad->y };
	normalizedTextureCoords = util::glNormalizeTextureCoords(
		{ calculatedTextureCoordinates.x, calculatedTextureCoordinates.y },
		{ texture->surface->w, texture->surface->h });
	vertex.textureCoordsAttribute = normalizedTextureCoords;

	spriteVertexBuffer.push_back(vertex);

	//v2
	vertex.positionAttribute = glm::vec3{ glSize.x, glSize.y, zIndex };
	vertex.colorAttribute = glColor;

	calculatedTextureCoordinates = { textureSrcQuad->x + textureSrcQuad->w, textureSrcQuad->y + textureSrcQuad->h };
	normalizedTextureCoords = util::glNormalizeTextureCoords(
		{ calculatedTextureCoordinates.x, calculatedTextureCoordinates.y },
		{ texture->surface->w, texture->surface->h });
	vertex.textureCoordsAttribute = normalizedTextureCoords;

	spriteVertexBuffer.push_back(vertex);

	//v3
	vertex.positionAttribute = glm::vec3{ 0, glSize.y, zIndex };
	vertex.colorAttribute = glColor;

	calculatedTextureCoordinates = { textureSrcQuad->x , textureSrcQuad->y + textureSrcQuad->h };
	normalizedTextureCoords = util::glNormalizeTextureCoords(
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


	if (GameConfig::instance().openGLBatching() == true) {
		_addVertexBufferToBatch(spriteVertexBuffer, GLDrawerType::GLSPRITE, texture, shadertype);
	}
	else {
		Shader shader = static_cast<GLRenderer*>(game->renderer())->shader(shadertype);
		m_spriteDrawer.draw(spriteVertexBuffer, spriteindexBuffer, shader, texture);

	}

	//Outline the object if defined so
	if (outline) {

		outlineObject(destQuad, outlineColor);
	}


}

void GLRenderer::drawLine(glm::vec2 pointA, glm::vec2 pointB, glm::uvec4 color)
{

	glm::vec4 normalizedcolor = util::glNormalizeColor(color);
	glm::vec4 redcolor = util::glNormalizeColor(glm::uvec4(255,0,0,255));

	//Array of 4 vertices
	std::vector<LineVertex> lineVertexBuffer;

	//
	// Vertex Buffer Data
	// 	   Build the 2 vertices that make a line
	// 
	int zIndex = -1;

	LineVertex vertex;
	//v0
	vertex.positionAttribute = glm::vec3{ pointA.x, pointA.y, zIndex };
	vertex.colorAttribute = normalizedcolor;
	lineVertexBuffer.push_back(vertex);

	//v1
	vertex.positionAttribute = glm::vec3{ pointB.x, pointB.y, zIndex };
	vertex.colorAttribute = normalizedcolor;
	lineVertexBuffer.push_back(vertex);

	//shader needs to be passed in
	auto shadertype = GLShaderType::LINE;

	if (GameConfig::instance().openGLBatching() == true) {
		_addVertexBufferToBatch(lineVertexBuffer, GLDrawerType::GLLINE, shadertype);
	}
	else {
		Shader shader = static_cast<GLRenderer*>(game->renderer())->shader(shadertype);
		m_lineDrawer.draw(lineVertexBuffer, 2, shader);
	}

}

GLRenderer::~GLRenderer()
{

	//glDeleteTextures(1, texture_id);

	//glDeleteVertexArrays(1, &m_vao);

    SDL_GL_DeleteContext(m_glcontext);

}

void GLRenderer::_addVertexBufferToBatch(const std::vector<SpriteVertex>& spriteVertices, GLDrawerType objectType, Texture* texture, GLShaderType shaderType)
{

	std::stringstream texturePtrString;
	std::stringstream keyString;

	if (texture == nullptr) {

		texturePtrString << "NO_TEXTURE";
	}
	else {
		texturePtrString << texture->surface;
	}
	
	//Build the map key
	keyString << (int)objectType << "_" << texturePtrString.str() << "_" << (int)shaderType;

	//See if the drawBatch for this combo exists yet
	if (m_drawBatches.find(keyString.str()) == m_drawBatches.end()) {

		auto spriteBatch = std::make_shared<SpriteDrawBatch>(objectType, texture, shaderType);
		spriteBatch->addVertexBuffer(spriteVertices);
		m_drawBatches[keyString.str()] = spriteBatch;

	}
	else {

		m_drawBatches[keyString.str()]->addVertexBuffer(spriteVertices);

	}


}


void GLRenderer::_addVertexBufferToBatch(const std::vector<LineVertex>& lineVertices, GLDrawerType objectType, GLShaderType shaderType)
{

	std::stringstream keyString;

	//Build the map key
	keyString << (int)objectType << "_" << (int)shaderType;

	//See if the drawBatch for this combo exists yet
	if (m_drawBatches.find(keyString.str()) == m_drawBatches.end()) {

		auto lineBatch = std::make_shared<LineDrawBatch>(shaderType);
		lineBatch->addVertexBuffer(lineVertices);
		m_drawBatches[keyString.str()] = lineBatch;

	}
	else {

		m_drawBatches[keyString.str()]->addVertexBuffer(lineVertices);

	}


}


void GLRenderer::prepTexture(Texture* texture)
{

	SDL_Surface* surf = texture->surface;
	GLenum texture_format{ GL_RGB };

	auto nOfColors = surf->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (surf->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_RGBA; //WEIRD PROBLEM assume always that the image is GL_RGBA
	}
	else if (nOfColors == 3)     // no alpha channel
	{
		if (surf->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}

	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//load in the image data
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, surf->w, surf->h, 0, texture_format, GL_UNSIGNED_BYTE, surf->pixels);

	return;
}

GLuint GLRenderer::getTextureId(GL_TextureIndexType textureIndex)
{

	GLuint textureId = m_textureIds[(int)textureIndex];
	return textureId;

}

void GLRenderer::renderPrimitives(int layerIndex)
{

	for (auto& line : m_primitiveLines) {

		drawLine(line.pointA, line.pointB, line.color);

	}

	m_primitiveLines.clear();

}

