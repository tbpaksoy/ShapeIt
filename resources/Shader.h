// En : This file contains the class and function prototypes for the Shader class.
// Tr : Bu dosya, Shader sınıfı için sınıf ve fonksiyon prototiplerini içerir.
#include <GL/glew.h>
#include <glm/glm.hpp>
#ifndef SHADER_H
#define SHADER_H

class Shader
{
private:
    GLuint vertexShader, fragmentShader, shaderProgram;

public:
    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);
    ~Shader();
    void Use();
    GLuint GetProgram() const;
    void SetUniform(const char *name, int value);
    void SetUniform(const char *name, unsigned int value);
    void SetUniform(const char *name, float value);
    void SetUniform(const char *name, glm::vec2 value);
    void SetUniform(const char *name, glm::vec3 value);
    void SetUniform(const char *name, glm::vec4 value);
    void SetUniform(const char *name, glm::mat4 value);
};

#endif