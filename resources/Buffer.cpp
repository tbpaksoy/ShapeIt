#include "Buffer.h"
#ifdef IMGUI
#include <iostream>

Buffer::Buffer()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}
Buffer(GLuint vao, int size, ImU32 renderType = GL_TRIANGLES, DrawType drawType = Elements)
    : vao(vao), size(size), renderType(renderType), drawType(drawType)
{
}
Buffer::Buffer(ImU32 vao, ImU32 vbo, ImU32 ebo, unsigned int size, ImU32 renderType, DrawType drawType)
    : vao(vao), vbo(vbo), ebo(ebo), size(size), renderType(renderType), drawType(drawType)
{
}

Buffer::~Buffer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Buffer::Draw()
{
    switch (drawType)
    {
    case Elements:
        glDrawElements(renderType, size, GL_UNSIGNED_INT, 0);
        break;
    case Array:
        glDrawArrays(renderType, 0, size);
        break;
    }
}

ImU32 Buffer::GetVAO() const
{
    return vao;
}

ImU32 Buffer::GetVBO() const
{
    return vbo;
}

ImU32 Buffer::GetEBO() const
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
    Draw();
}
#else

Buffer::Buffer()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}
Buffer::Buffer(GLuint vao, int size, GLuint renderType, DrawType drawType)
    : vao(vao), size(size), renderType(renderType), drawType(drawType)
{
}
Buffer::Buffer(GLuint vao, GLuint vbo, GLuint ebo, unsigned int size, GLuint renderType, DrawType drawType)
    : vao(vao), vbo(vbo), ebo(ebo), size(size), renderType(renderType), drawType(drawType)
{
}
Buffer::~Buffer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
void Buffer::Draw()
{
    switch (drawType)
    {
    case Elements:
        glDrawElements(renderType, size, GL_UNSIGNED_INT, 0);
        break;
    case Array:
        glDrawArrays(renderType, 0, size);
        break;
    }
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
    Draw();
}
#endif