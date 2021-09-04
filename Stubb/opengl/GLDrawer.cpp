#include "GLDrawer.h"


GLDrawer::GLDrawer(GLDrawerType glDrawerType)
{

	//Generate and bind to a new VOA (Vertex Object Array)
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);

	//Generate and bind to a VBO
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

	//_setVertexBufferAttriubuteLayout(glDrawerType);




}

GLDrawer::~GLDrawer()
{
}
