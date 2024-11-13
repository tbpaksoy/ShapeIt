#ifndef BUFFER_H
#define BUFFER_H

#ifdef IMGUI

#else
#include <GL/glew.h>
#include <vector>
#include <functional>

struct Buffer
{
private:
    GLuint vao, vbo, ebo;
    unsigned int size;

public:
    Buffer();
    Buffer(GLuint vao, GLuint vbo, GLuint ebo = 0, unsigned int size = 0);
    ~Buffer();
    GLuint GetVAO() const;
    GLuint GetVBO() const;
    GLuint GetEBO() const;
    unsigned int GetSize() const;
    void Bind();
    void Render();
};
#endif

#endif