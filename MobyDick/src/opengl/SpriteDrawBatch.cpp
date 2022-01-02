#include "SpriteDrawBatch.h"

#include <memory>
#include <glad/glad.h>
#include "GLRenderer.h"
#include "../game.h"
#include "Vertex.h"

extern std::unique_ptr<Game> game;

SpriteDrawBatch::SpriteDrawBatch(GLDrawerType objectType, Texture* texture, GLShaderType shaderType)
{

	switch (objectType) {
	case GLDrawerType::GLSPRITE:
		m_glDrawer = static_cast<GLRenderer*>(game->renderer())->spriteDrawer();
		break;
	}

	m_shader = static_cast<GLRenderer*>(game->renderer())->shader(shaderType);

	m_texture = texture;

}

void SpriteDrawBatch::addVertexBuffer(const std::vector<SpriteVertex>& spriteVertices)
{

	for (const auto& vertex : spriteVertices) {

		m_vertexBatch.push_back(vertex);
	}

	//index buffer 
	_addSpriteIndexBuffer();

	m_objectCount++;

}

void SpriteDrawBatch::clear()
{
	m_vertexBatch.clear();
	m_indexes.clear();

	m_objectCount = 0;

}

void SpriteDrawBatch::draw()
{

	m_glDrawer.draw(m_vertexBatch, m_indexes, m_shader, m_texture);


}

void SpriteDrawBatch::_addSpriteIndexBuffer()
{
	//Sprite Index format
	//0,1,2, top right triangle
	//2,3,0, bottom left triangle

	//Create the new set of 6 index items
	for (int i = 0; i < 6; i++) {
		m_indexes.push_back(spriteindexBuffer[i] + (m_objectCount*4));
	}
}


