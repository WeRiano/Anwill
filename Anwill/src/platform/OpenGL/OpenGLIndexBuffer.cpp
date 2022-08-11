#include "platform/OpenGL/OpenGLIndexBuffer.h"
#include "core/Assert.h"

namespace Anwill {

    OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* indices, unsigned int count)
        : m_Count(count)
    {
        AW_ASSERT(sizeof(unsigned int) == sizeof(GLuint),
                  "size of GLuint =/= size of regular unsigned int");

        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices,
                     GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int count)
            : m_Count(count)
    {
        AW_ASSERT(sizeof(unsigned int) == sizeof(GLuint),
                  "size of GLuint =/= size of regular unsigned int");

        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr,
                     GL_DYNAMIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void OpenGLIndexBuffer::DynamicUpdate(const void* data, unsigned int size)
    {
        Bind();
        m_Count = size;
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(unsigned int), data);
        Unbind();
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    unsigned int OpenGLIndexBuffer::GetCount() const
    {
        return m_Count;
    }
}