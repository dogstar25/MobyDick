#pragma once
#include "DrawBatch.h"
#include <memory>

#include "GLDrawer.h"
#include "Vertex.h"
#include "SpriteVertex.h"
//#include "IndexBuffer.h"
#include "Shader.h"
#include "../texture.h"

//temp
//struct SpriteindexBuffer {
//	glm::uvec3 topLeftTriangle{ 0,1,2 };
//	glm::uvec3 botRightTriangle{ 2,3,0 };
//};

const int spriteindexBuffer[] = { 0,1,2,2,3,0 };


class SpriteDrawBatch : public DrawBatch
{

public:

	SpriteDrawBatch() = default;
	SpriteDrawBatch(GLDrawerType ObjectType, Texture * texture, GLShaderType shaderType);

	void addVertexBuffer(const std::vector<SpriteVertex>& spriteVertices) override;
	void _addSpriteIndexBuffer() override;
	void draw() override ;
	void clear() override;
//	std::vector<glm::uint> const& indexes() { return  m_indexes; }

	//void setShaderId(int shaderId) { mShaderId = shaderId; }
	//void setTextureId()


private:
	//int mObjectType{}; //Sprite, Line, pixel, triangle
	/*GLuint mShaderId{};
	GLuint mTextureId{};*/

	std::vector<SpriteVertex> m_vertexBatch{};
	//std::vector<glm::uint> m_indexes;
	//std::shared_ptr<GLDrawer> m_glDrawer;
	//std::shared_ptr<Shader> m_shader;
	Texture* m_texture;

	//int m_objectCount{};

	void prepTexture();


};

