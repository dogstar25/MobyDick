#pragma once

#define GL_GLEXT_PROTOTYPES

#include <memory>
#include <vector>
#include <unordered_map>
#include <array>

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLDebugCallback.h"
#include "GLDrawer.h"
#include "../Renderer.h"
#include "SpriteVertex.h"
#include "Vertex.h"
#include "Shader.h"
#include "../BaseConstants.h"

class DrawBatch;

enum class GL_TextureIndexType {
	MAIN_TEXTURE_ATLAS = 0,
	DYNAMICALLY_LOADED,

	LAST_INDEX,
	COUNT
};

class GLRenderer : public Renderer
{

	inline static constexpr int MAX_TEXTURE_ATLAS = 4;

public:
	GLRenderer();
	~GLRenderer();

	SDL_Renderer* sdlRenderer() { return nullptr; }
	void init(SDL_Window* window);
	bool present();
	bool clear();
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface) { return nullptr; };
	//Note:the renderComponent will call this draw
	void drawSprite(int objectType, glm::vec2 position, GLint layer, GLfloat angle, GLfloat width, GLfloat height, glm::vec4 color, GLuint textureId, glm::vec2 textureCoords );
	void drawSprite(SDL_FRect quad, SDL_Color color, int layer, Texture* texture,
		SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor) override;
	void drawLine(b2Vec2 lineStart, b2Vec2 lineEnd, SDL_Color color) {};
	void drawQuad(SDL_FRect quad, SDL_Color color, bool outline, SDL_Color outlineColor);
	std::shared_ptr<GLDrawer> spriteDrawer(){ return m_spriteDrawer; }
	void bindTexture(Texture* texture);
	//void prepTexture(int openGlTextureIndex, Texture* texture);
	void prepTexture(Texture* texture);
	GLuint getTextureId(GL_TextureIndexType textureindex);


	Shader shader(GLShaderType shaderType) {
		return m_shaders[(int)shaderType];
	}

	glm::mat4  projectionMatrix() { return m_projectionMatrix; }

private:

	void _setVertexBufferAttriubuteLayout();
	void _addVertexBuffer(const std::vector<SpriteVertex>& spriteVertices, int layer, GLDrawerType objectType, Texture* texture, GLShaderType shaderType);
	void _addVertexBuffer(std::vector<std::shared_ptr<Vertex>>);
	
	GLuint _addTexture(Texture* texture);

	//void prepTextures();

	SDL_GLContext m_glcontext{};

	//GLDrawer m_glDrawers[static_cast<int>(GLDrawerType::count) + 1];

	std::shared_ptr<GLDrawer> m_spriteDrawer;
	std::shared_ptr<GLDrawer> m_lineDrawer;

	//Projection matrix
	glm::mat4  m_projectionMatrix{1.0};

	std::map<std::string, std::shared_ptr<DrawBatch>> m_drawBatches;
	std::array<Shader, int(GLShaderType::count) +1> m_shaders;



	//this should be an array of texturePointerAddr's
	//The index in is array of where it is stored will be the same index that
	//represents the opengl textures
	std::array<std::string, MAX_TEXTURE_ATLAS> m_currentTextures{};
	
	GLuint m_textureIds[MAX_TEXTURE_ATLAS];

	

	
};

