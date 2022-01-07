#pragma once
#include "DrawBatch.h"
#include <memory>

#include "GLDrawer.h"
#include "Vertex.h"
#include "Shader.h"
#include "../texture.h"


const int spriteindexBuffer[] = { 0,1,2,2,3,0 };


class SpriteDrawBatch : public DrawBatch
{

public:

	SpriteDrawBatch() = default;
	SpriteDrawBatch(GLDrawerType ObjectType, Texture * texture, GLShaderType shaderType, RenderBlendMode textureBlendMode);

	void addVertexBuffer(const std::vector<SpriteVertex>& spriteVertices) override;
	void _addSpriteIndexBuffer();
	void draw() override ;
	void clear() override;


private:

	std::vector<SpriteVertex> m_vertexBatch{};
	RenderBlendMode m_textureBlendMode;
	Texture* m_texture;

};

