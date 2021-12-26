#include "GLDrawer.h"

#include "SpriteVertex.h"


GLDrawer::GLDrawer(GLDrawerType drawerType)
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

	switch (drawerType){
		case GLDrawerType::GLSPRITE:
			m_vertexSize = sizeof(SpriteVertex);
			break;
		case GLDrawerType::GLLINE:
			//m_vertexSize = sizeof(SpriteVertex);
			break;
	}



}

GLDrawer::~GLDrawer()
{
}

void GLDrawer::prepare()
{

	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);


}

void GLDrawer::bind()
{

}

void GLDrawer::_setVertexBufferAttriubuteLayout(GLDrawerType drawerType)
{

	if (drawerType == GLDrawerType::GLSPRITE) {

		// vertex attrubute indexes
		const int attrib_position = 0;
		const int attrib_color = 1;
		const int attrib_texture = 2;
		const int attrib_texture_index = 3;

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0);
		glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(3 * sizeof(float)));
		glVertexAttribPointer(attrib_texture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(7 * sizeof(float)));
		glVertexAttribPointer(attrib_texture_index, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(9 * sizeof(float)));


	}
	else if (drawerType == GLDrawerType::GLLINE) {

	}


}
