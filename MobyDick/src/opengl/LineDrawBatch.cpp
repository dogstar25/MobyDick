#include "LineDrawBatch.h"

#include <memory>
#include <glad/glad.h>
#include "GLRenderer.h"
#include "../game.h"
#include "Vertex.h"

extern std::unique_ptr<Game> game;

LineDrawBatch::LineDrawBatch(GLShaderType shaderType)
{

	m_glDrawer = static_cast<GLRenderer*>(game->renderer())->lineDrawer();
	m_shader = static_cast<GLRenderer*>(game->renderer())->shader(shaderType);

}

void LineDrawBatch::addVertexBuffer(const std::vector<LineVertex>& lineVertices)
{

	for (const auto& vertex : lineVertices) {

		m_vertexBatch.push_back(vertex);
	}

	m_objectCount++;

}

void LineDrawBatch::clear()
{
	m_vertexBatch.clear();
	m_indexes.clear();

	m_objectCount = 0;

}

void LineDrawBatch::draw()
{
	//Draw the lines - each line has 2 points , hence * 2
	m_glDrawer.draw(m_vertexBatch, m_objectCount * 2, m_shader);


}


