#include <time.h>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <SDL/SDL.h>
#include "resources/Window.h"
#include "resources/Window.cpp"
#include "resources/Buffer.h"
#include "resources/Buffer.cpp"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    glfwInit();
    SDL_Init(SDL_INIT_VIDEO);

    srand(time(NULL));

    Window *window0 = new Window("Window Test", 0, 0);

    window0->Render();

    delete window0;
}