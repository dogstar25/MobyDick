#pragma once
#include <glad/glad.h>

class IndexBuffer
{

private:

	GLuint m_rendererId{};
	GLuint m_count{};

public:
	IndexBuffer(const GLuint *data, GLuint count);
	~IndexBuffer();

	void bind();
	void unbind();

	GLuint rendererId() { return m_rendererId; }

	inline GLuint count() const { return m_count; }

};

