#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//const GLfloat g_vertex_buffer_data[] = {
//	/*  x   y    z   R, G, B, A,  txX, txY */
//		50, 50, -1,  1, 0, 0, 1,  0.0, 0.0, // vertex 0
//		75, 50, -1,  1, 0, 0, 1,  1.0, 0.0, // vertex 1
//		75, 75, -1,  1, 0, 0, 1,  1.0, 1.0, // vertex 2
//		50, 75, -1,  1, 0, 0, 1,  0.0, 1.0  // vertex 3
//};

//struct SpriteVertex {
//
//	glm::vec3 positionAttribute;
//	glm::vec4 colorAttribute;
//	glm::vec2 textureCoordsAttribute;
//};
//
//struct SpriteindexBuffer {
//	glm::uvec3 topLeftTriangle{0,1,2};
//	glm::uvec3 botRightTriangle{2,3,0};
//};


class VertexBuffer
{

private:

	GLuint m_rendererId;
	std::vector<SpriteVertex> m_spriteBuffer;



public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void bind();
	void unbind();

	void addVertex(GLuint bufferPosition, const SpriteVertex* vertex);

	GLuint rendererId() { return m_rendererId; }

	



};

