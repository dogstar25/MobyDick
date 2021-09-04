#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, GLuint size)
{

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

void VertexBuffer::addVertex(GLuint bufferPosition, const SpriteVertex* vertex)
{

    m_spriteBuffer.push_back(*vertex);


}
