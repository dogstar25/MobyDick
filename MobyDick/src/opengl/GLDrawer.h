#pragma once

#include <glad/glad.h>

#include "../BaseConstants.h"

class GLDrawer
{

public:

	GLDrawerType drawerType;

	GLDrawer() = default;
	GLDrawer(GLDrawerType);
	~GLDrawer();

private:
	
	GLuint m_vaoId;
	GLuint m_vboId;
	GLuint m_iboId;

	void _setVertexBufferAttriubuteLayout(GLDrawerType);


};

