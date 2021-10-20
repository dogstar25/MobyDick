#pragma once
#include <vector>
#include <memory>
#include <glad/glad.h>

#include "GLDrawer.h"
#include "Vertex.h"
#include "SpriteVertex.h"
//#include "IndexBuffer.h"
#include "Shader.h"
#include "../texture.h"

class DrawBatch
{
public:

	DrawBatch() = default;
	
	virtual void addVertexBuffer(const std::vector<SpriteVertex>& spriteVertices) = 0;
	virtual void draw() = 0;
	virtual void clear() = 0;
	
	std::vector<glm::uint> const& indexes() { return  m_indexes; }

protected:

	std::vector<glm::uint> m_indexes;
	std::shared_ptr<GLDrawer> m_glDrawer;
	std::shared_ptr<Shader> m_shader;
	int m_objectCount{};

	virtual void _addSpriteIndexBuffer() = 0;

};

