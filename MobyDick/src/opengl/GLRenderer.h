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

	inline static constexpr int MAX_TEXTURES_IDS = 4;
	const std::vector<glm::uint> spriteindexBuffer{ 0,1,2,2,3,0 };
	const std::vector<glm::uint> lineindexBuffer{ 0,1};


public:
	GLRenderer();
	~GLRenderer();

	SDL_Renderer* sdlRenderer() { return nullptr; }
	void init(SDL_Window* window);
	bool present();
	bool clear();
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface) { return nullptr; };
	void drawSprite(SDL_FRect quad, SDL_Color color, Texture* texture, SDL_Rect* textureSrcQuad, float angle, bool outline, SDL_Color outlineColor) override;
	void drawLine(glm::vec2 pointA, glm::vec2 pointB, glm::uvec4 color);
	const GLDrawer& spriteDrawer(){ return m_spriteDrawer; }
	void prepTexture(Texture* texture);
	GLuint getTextureId(GL_TextureIndexType textureindex);
	void renderPrimitives(int layerIndex);


	const Shader& shader(GLShaderType shaderType) {
		return m_shaders[(int)shaderType];
	}

	glm::mat4  projectionMatrix() { return m_projectionMatrix; }

private:

	void _addVertexBufferToBatch(const std::vector<SpriteVertex>& spriteVertices, GLDrawerType objectType, Texture* texture, GLShaderType shaderType);
	void _addVertexBufferToBatch(const std::vector<LineVertex>& lineVertices, GLDrawerType objectType, GLShaderType shaderType);
	
	GLuint _addTexture(Texture* texture);
	SDL_GLContext m_glcontext{};
	GLDrawer m_spriteDrawer;
	GLDrawer m_lineDrawer;

	//Projection matrix
	glm::mat4  m_projectionMatrix{1.0};

	std::map<std::string, std::shared_ptr<DrawBatch>> m_drawBatches;
	std::array<Shader, int(GLShaderType::count) +1> m_shaders;

	GLuint m_textureIds[MAX_TEXTURES_IDS];

	

	
};

