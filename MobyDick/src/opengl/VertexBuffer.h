#pragma once
#include <glad/glad.h>

class VertexBuffer
{

private:

	GLuint m_rendererId;

public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void bind();
	void unbind();

	GLuint rendererId() { return m_rendererId; }



};

