#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count)
    :m_count(count)
{

    glGenBuffers(1, &m_rendererId);

    //Bind the vertext buffer array to the GL_ARRAY_BUFFER
    //we can only bind ONE vertex buffer objewct (vbo) to the GL_ARRAY_BUFFER buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId); //state setting

    //We are now bound to the GL_ARRAY_BUFFER buffer
    //Copy the vertex data into the buffer bound to the GL_ARRAY_BUFFER, which is vbo
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW); // state using



}

IndexBuffer::~IndexBuffer()
{

    glDeleteBuffers(1, &m_rendererId);

}

void IndexBuffer::bind()
{

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void IndexBuffer::unbind()
{

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
