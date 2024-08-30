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
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
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