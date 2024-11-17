#include <vector>
#include <functional>
#ifndef BUFFER_H
#define BUFFER_H

enum DrawType
{
    Elements,
    Array
};

#ifdef IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#else
#include <GL/glew.h>
#endif
struct Buffer
{
private:
#ifdef IMGUI
    ImU32 vao, vbo, ebo, renderType;
#else
    GLuint vao, vbo, ebo, renderType;
#endif
    unsigned int size;
    DrawType drawType;
    void Draw();

public:
    Buffer();
#ifdef IMGUI
    Buffer(GLuint vao, int size, ImU32 renderType = GL_TRIANGLES, DrawType drawType = Elements);
    Buffer(GLuint vao, GLuint vbo, GLuint ebo = 0, unsigned int size = 0, ImU32 renderType = GL_TRIANGLES, DrawType drawType = Elements);
#else
    Buffer(GLuint vao, int size, GLuint renderType = GL_TRIANGLES, DrawType drawType = Elements);
    Buffer(GLuint vao, GLuint vbo, GLuint ebo = 0, unsigned int size = 0, GLuint renderType = GL_TRIANGLES, DrawType drawType = Elements);
#endif
    ~Buffer();
#ifdef IMGUI
    ImU32 GetVAO() const;
    ImU32 GetVBO() const;
    ImU32 GetEBO() const;
#else
    GLuint GetVAO() const;
    GLuint GetVBO() const;
    GLuint GetEBO() const;
#endif
    unsigned int GetSize() const;
    void Bind();
    void Render();
};

#endif