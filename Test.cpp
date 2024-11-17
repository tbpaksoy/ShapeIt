#include <iostream>

#include <exprtk.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "resources/Shader.h"
#include "resources/Shader.cpp"

#include "resources/Operator.h"
#include "resources/Operator.cpp"

#include "resources/Helpers.h"
#include "resources/Helpers.cpp"

#include "resources/Camera.h"
#include "resources/Camera.cpp"

#include <windows.h>
#undef IMGUI
#include "resources/Window.h"
#include "resources/Window.cpp"
#include "resources/Buffer.h"
#include "resources/Buffer.cpp"

void WriteError()
{
    GLenum error = glGetError();

    std::cout << "Error: " << error << " " << glewGetErrorString(error) << std::endl;
}
#ifndef WINDOW_H
int main()
{
    int segSize;
    float gridlength;

    std::cout << "Enter the number of segments: ";
    std::cin >> segSize;
    std::cout << "Enter the length of the grid: ";
    std::cin >> gridlength;

    if (!glfwInit())
    {
        std::cout << "GLFW could not be initialized." << std::endl;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(1000, 1000, "Test", nullptr, nullptr);

    if (!window)
        std::cout << "Window could not be created." << std::endl;

    glfwMakeContextCurrent(window);

    glewInit();

    int vSize, iSize;
    float *vertices = CreateGridVertices(gridlength, segSize, vSize, true, true, true);
    int *indices = CreateGridIndices(segSize, iSize, true, true, true);

    GLuint VAO;
    PrepareGrid(VAO, vertices, vSize, indices, iSize);

    Shader *shader = new Shader("Shaders\\vertex.vs", "Shaders\\fragment.fs");

    Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, -2.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f));

    shader->Use();
    shader->SetUniform("model", camera->GetModelMatrix());
    shader->SetUniform("view", camera->GetViewMatrix());
    shader->SetUniform("projection", camera->GetProjectionMatrix());

    float currentFrame = 0.0f, lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT);

        camera->Move(glm::vec3(1.0f, 1.0f, -1.0f) * deltaTime * 0.01f);

        shader->SetUniform("model", camera->GetModelMatrix());
        shader->SetUniform("view", camera->GetViewMatrix());
        shader->SetUniform("projection", camera->GetProjectionMatrix());

        shader->Use();

        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, iSize, GL_UNSIGNED_INT, 0);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
#else
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    int segSize;
    float gridlength;

    std::cout << "Enter the number of segments: ";
    std::cin >> segSize;
    std::cout << "Enter the length of the grid: ";
    std::cin >> gridlength;

    glfwInit();

    Window *window = new Window("Test", 1000, 1000);

    glewInit();

    int vSize, iSize;
    float *vertices = CreateGridVertices(gridlength, segSize, vSize, true, true, true);
    int *indices = CreateGridIndices(segSize, iSize, true, true, true);

    GLuint VAO;
    PrepareGrid(VAO, vertices, vSize, indices, iSize);

    Buffer buffer(VAO, 0, 0, iSize, GL_LINES, Elements);

    Shader *shader = new Shader("Shaders\\vertex.vs", "Shaders\\fragment.fs");

    Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, -2.0f), glm::quat(glm::vec3(0.0f)), glm::vec3(1.0f));

    shader->Use();
    shader->SetUniform("model", camera->GetModelMatrix());
    shader->SetUniform("view", camera->GetViewMatrix());
    shader->SetUniform("projection", camera->GetProjectionMatrix());

    window->AddBuffer(buffer);

    window->AddOnFrame([&]
                       {
                           camera->Move(glm::vec3(1.0f, 1.0f, -1.0f) * window->GetDeltaTime() * 0.01f);

                           shader->Use();
                           shader->SetUniform("model", camera->GetModelMatrix());
                           shader->SetUniform("view", camera->GetViewMatrix());
                           shader->SetUniform("projection", camera->GetProjectionMatrix()); 
                           shader->Use(); });

    window->Render();
}
#endif