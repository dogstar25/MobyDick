#pragma once
#include <glad/glad.h>

#include <vector>

const GLfloat g_vertex_buffer_data[] = {
	/*  x   y    R, G, B, A,  txX, txY */
		50, 50,  1, 0, 0, 1,  0.0, 0.0, // vertex 0
		75, 50,  1, 0, 0, 1,  1.0, 0.0, // vertex 1
		75, 75,  1, 0, 0, 1,  1.0, 1.0, // vertex 2
		50, 75,  1, 0, 0, 1,  0.0, 1.0  // vertex 3
};

struct SpriteVertex {

	GLfloat positionAttribute[2];
	GLfloat colorAttribute[4];
	GLfloat textureCoordsAttribute[2];
};

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

	void addVertex(GLuint bufferPosition, const GLfloat* vertex);

	GLuint rendererId() { return m_rendererId; }



};

