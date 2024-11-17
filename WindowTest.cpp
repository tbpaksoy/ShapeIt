#define IMGUI
#include <time.h>
#include <random>
#ifndef IMGUI
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <windows.h>
#include <SDL/SDL.h>
#undef GL_VERSION_1_1
#include "resources/Window.h"
#include "resources/Window.cpp"
#undef GL_VERSION_1_1
#include "resources/Buffer.h"
#include "resources/Buffer.cpp"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    glfwInit();

    srand(time(NULL));

    Window *window = new Window("Window Test", 0, 0, glfwGetPrimaryMonitor());

    int xSize, ySize;
    ySize = window->GetHeight();
    xSize = window->GetWidth();

    const char *str = " ";

    window->AddImGuiRender([&]()
                           {
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(xSize / 8,ySize));
        ImGui::Begin("Test 1", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Hello, world!");
        ImGui::Button("Test",ImVec2(50,50));
        ImGui::End(); });
    window->AddImGuiRender([&]()
                           {
        ImGui::SetNextWindowPos(ImVec2(xSize / 8,0));
        ImGui::SetNextWindowSize(ImVec2(6 * xSize / 8,ySize / 6.0f));
        ImGui::Begin("Test2", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);



        for(int i = 1; i <= 5; i++ )
        {
            ImGui::SameLine();
            if(ImGui::Button((std::string("Button ") + std::to_string(i)).c_str() ,ImVec2(75,75)))
            {
                std::string s("Button ");
                s.append(std::to_string(i));
                s.append(" pressed.");
                str = new char[s.size()];
                str = s.c_str();
            }
        }

        ImGui::Text(str);

        ImGui::End(); });
    window->AddImGuiRender([&]()
                           {
        ImGui::SetNextWindowPos(ImVec2(xSize / 8, 5 * ySize / 6));
        ImGui::SetNextWindowSize(ImVec2(6 * xSize / 8, ySize / 6));
        ImGui::Begin("Test3", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::End(); });
    window->AddImGuiRender([&]()
                           {
        ImGui::SetNextWindowPos(ImVec2(7 * xSize / 8, 0));
        ImGui::SetNextWindowSize(ImVec2(xSize / 8, ySize));
        ImGui::Begin("Test4", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::End(); });

    window->Render();

    delete window;
}