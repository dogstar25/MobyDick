#pragma once

#include <glad/glad.h>

#include <vector>

#include "SpriteVertex.h"
#include "Shader.h"
//#include "../texture.h"
#include "../BaseConstants.h"

class Texture;

class GLDrawer
{

public:

	GLDrawerType drawerType;

	GLDrawer() = default;
	GLDrawer(GLDrawerType drawerType);
	~GLDrawer();

	void prepare();
	void draw(const std::vector<SpriteVertex>& spriteVertices, const std::vector<glm::uint>& spriteVertexIndexes, Shader& shader, Texture* texture);
	void bind();

private:
	
	GLuint m_vaoId{};
	GLuint m_vboId{};
	GLuint m_iboId{};
	float m_vertexSize{};

	void _setVertexBufferAttriubuteLayout(GLDrawerType drawerType);


};

