#include "Shader.h"
#include <fstream>
#include <iostream>

Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource)
{

    std::ifstream vertexFile, fragmentFile;
    try
    {
        vertexFile.open(vertexShaderSource);
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: vertex shader file not found or valid." << std::endl;
    }
    try
    {
        fragmentFile.open(fragmentShaderSource);
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: fragment shader file not found or valid." << std::endl;
    }

    std::string vertexCode, fragmentCode;
    std::string line;
    while (std::getline(vertexFile, line))
    {
        vertexCode += line + "\n";
    }
    while (std::getline(fragmentFile, line))
    {
        fragmentCode += line + "\n";
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error: " << error << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);

    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error: " << error << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);

    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error: " << error << std::endl;
    }

    glAttachShader(shaderProgram, fragmentShader);

    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error: " << error << std::endl;
    }

    glLinkProgram(shaderProgram);

    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error: " << error << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(shaderProgram);
}

void Shader::Use()
{
    glUseProgram(shaderProgram);
}

GLuint Shader::GetProgram() const
{
    return shaderProgram;
}

void Shader::SetUniform(const char *name, int value)
{
    Use();
    int loc = glGetUniformLocation(shaderProgram, name);
    if (loc == -1)
    {
        std::cout << "Error: Uniform not found." << std::endl
                  << name << std::endl;
    }
    glUniform1i(loc, value);
}
void Shader::SetUniform(const char *name, unsigned int value)
{
    Use();
    int loc = glGetUniformLocation(shaderProgram, name);
    if (loc == -1)
    {
        std::cout << "Error: Uniform not found." << std::endl
                  << name << std::endl;
    }
    glUniform1ui(loc, value);
}
void Shader::SetUniform(const char *name, float value)
{
    Use();
    int loc = glGetUniformLocation(shaderProgram, name);
    if (loc == -1)
    {
        std::cout << "Error: Uniform not found." << std::endl
                  << name << std::endl;
    }
    glUniform1f(loc, value);
}
void Shader::SetUniform(const char *name, glm::vec2 value)
{
    Use();
    int loc = glGetUniformLocation(shaderProgram, name);
    if (loc == -1)
    {
        std::cout << "Error: Uniform not found." << std::endl
                  << name << std::endl;
    }
    glUniform2f(loc, value.x, value.y);
}
void Shader::SetUniform(const char *name, glm::vec3 value)
{
    Use();
    int loc = glGetUniformLocation(shaderProgram, name);
    if (loc == -1)
    {
        std::cout << "Error: Uniform not found." << std::endl
                  << name << std::endl;
    }
    glUniform3f(loc, value.x, value.y, value.z);
}
void Shader::SetUniform(const char *name, glm::vec4 value)
{
    Use();
    int loc = glGetUniformLocation(shaderProgram, name);
    if (loc == -1)
    {
        std::cout << "Error: Uniform not found." << std::endl
                  << name << std::endl;
    }

    glUniform4f(loc, value.x, value.y, value.z, value.w);
}
void Shader::SetUniform(const char *name, glm::mat4 value)
{
    Use();
    int loc = glGetUniformLocation(shaderProgram, name);
    if (loc == -1)
    {
        std::cout << "Error: Uniform not found." << std::endl
                  << name << std::endl;
    };
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}