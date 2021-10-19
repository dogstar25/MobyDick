#pragma once

#include <glad/glad.h>

#include "../BaseConstants.h"

class GLDrawer
{

public:

	GLDrawerType drawerType;

	GLDrawer() = default;
	GLDrawer(GLDrawerType drawerType);
	~GLDrawer();

	void prepare();
	void bind();

private:
	
	GLuint m_vaoId{};
	GLuint m_vboId{};
	GLuint m_iboId{};
	float m_vertexSize{};

	void _setVertexBufferAttriubuteLayout(GLDrawerType drawerType);


};

