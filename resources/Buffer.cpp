#include "Buffer.h"
#ifdef IMGUI

#else

Buffer::Buffer()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}
Buffer::Buffer(GLuint vao, GLuint vbo, GLuint ebo, unsigned int size)
    : vao(vao), vbo(vbo), ebo(ebo), size(size)
{
}
Buffer::~Buffer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
GLuint Buffer::GetVAO() const
{
    return vao;
}
GLuint Buffer::GetVBO() const
{
    return vbo;
}
GLuint Buffer::GetEBO() const
{
    return ebo;
}
unsigned int Buffer::GetSize() const
{
    return size;
}
void Buffer::Bind()
{
    glBindVertexArray(vao);
}
void Buffer::Render()
{
    Bind();
    if (ebo && size)
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, size);
}
#endif