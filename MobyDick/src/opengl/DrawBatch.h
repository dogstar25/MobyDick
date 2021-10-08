#pragma once
#include <glad/glad.h>
#include <vector>
#include <memory>

#include "GLDrawer.h"
#include "Vertex.h"
#include "SpriteVertex.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "../texture.h"

//temp
struct SpriteindexBuffer {
	glm::uvec3 topLeftTriangle{ 0,1,2 };
	glm::uvec3 botRightTriangle{ 2,3,0 };
};


class DrawBatch
{
public:

	DrawBatch() = default;
	DrawBatch(GLDrawerType ObjectType, Texture* texture, GLShaderType shaderType);
	
	void addVertexBuffer(std::vector<SpriteVertex> spriteVertices);
	void draw();
	void clear();
	//void setShaderId(int shaderId) { mShaderId = shaderId; }
	//void setTextureId()


private:
	//int mObjectType{}; //Sprite, Line, pixel, triangle
	/*GLuint mShaderId{};
	GLuint mTextureId{};*/

	std::vector<SpriteVertex> m_vertexBatch{};
	IndexBuffer m_indexBufferBatch{};
	std::shared_ptr<GLDrawer> m_glDrawer;

	std::shared_ptr<Shader> m_shader;
	Texture* m_texture;

	GLuint prepTexture();

};

