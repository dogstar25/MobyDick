#include "GLDrawer.h"


GLDrawer::GLDrawer(int drawerType)
{

	//Generate and bind to a new VOA (Vertex Object Array)
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);

	//Generate and bind to a VBO
	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

	_setVertexBufferAttriubuteLayout(drawerType);

	//Generate a IBO
	glGenBuffers(1, &m_iboId);

	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

GLDrawer::~GLDrawer()
{
}

void GLDrawer::_setVertexBufferAttriubuteLayout(int drawerType)
{

	if (drawerType == GameObjectType::SPRITE) {

		// vertex attrubute indexes
		const int attrib_position = 0;
		const int attrib_color = 1;
		const int attrib_texture = 2;

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
		glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
		glVertexAttribPointer(attrib_texture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(7 * sizeof(float)));


	}
	else if (drawerType == GameObjectType::LINE) {

	}


}
