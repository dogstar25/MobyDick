#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

VertexBuffer::VertexBuffer(const void* data, GLuint size)
{

    glGenBuffers(1, &m_rendererId);

    //Bind the vertext buffer array to the GL_ARRAY_BUFFER
    //we can only bind ONE vertex buffer objewct (vbo) to the GL_ARRAY_BUFFER buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId); //state setting

    //We are now bound to the GL_ARRAY_BUFFER buffer
    //Copy the vertext data into the buffer bound to the GL_ARRAY_BUFFER, which is vbo
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // state using

}

VertexBuffer::~VertexBuffer()
{

    glDeleteBuffers(1, &m_rendererId);

}

void VertexBuffer::bind()
{

    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void VertexBuffer::unbind()
{

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 100.0f);

    auto test = static_cast<glm::f32mat4>(proj);

    //glUniformMatrix4fv(0,1,false )
    //glUniformMat4f

    

    
}
