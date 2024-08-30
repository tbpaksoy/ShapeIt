#include <GL/glew.h>
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
};
#endif