#include "Window.h"
#ifdef IMGUI
#else

#include <SDL/SDL.h>

// FIXME: This function is not working properly (4.11.2024)
void Render(Window **windows, int count)
{
    bool oneClosed = false;
    while (true)
    {
        for (int i = 0; i < count; i++)
        {
            oneClosed |= glfwWindowShouldClose(windows[i]->window);
        }
        if (oneClosed)
            break;
        for (int i = 0; i < count; i++)
        {
            glfwMakeContextCurrent(windows[i]->window);
            windows[i]->Render();
        }
    }
}

// En: Constructor and destructor
// Tr: Yapıcı ve yıkıcı fonksiyonlar

Window::Window(const char *title, int width, int height, GLFWmonitor *monitor,
               bool resizable)
{
    if (width < 1 || height < 1)
    {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        width = mode->width;
        height = mode->height;
    }
    this->title = title;
    this->width = width;
    this->height = height;
    this->resizable = resizable;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, monitor, nullptr);
}
Window::~Window()
{
    glfwTerminate();
}

// En: Functions to get and update window related variables
// Tr: Pencere ile ilgili değişkenleri almak ve güncellemek için fonksiyonlar

const char *Window::GetTitle() const
{
    return title;
}
int Window::GetWidth() const
{
    return width;
}
int Window::GetHeight() const
{
    return height;
}
void Window::UpdateTitle(const char *title)
{
    this->title = title;
    glfwSetWindowTitle(window, title);
}
void Window::UpdateResizable(bool resizable)
{
    this->resizable = resizable;
    glfwWindowHint(GLFW_RESIZABLE, resizable);
}
void Window::SetFullscreen()
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}
void Window::SetFullscreen(GLFWmonitor *monitor)
{
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

// En: Functions related to input
// Tr: Girdi ile ilgili fonksiyonlar

bool Window::MouseButton(int button, int &action)
{
    action = glfwGetMouseButton(window, button);
    return action;
}
bool Window::KeyPress(int key, int &action)
{
    action = glfwGetKey(window, key);
    return action;
}
void Window::MousePosition(float &x, float &y)
{
    x = mouseX;
    y = mouseY;
}

// En: Functions related to rendering
// Tr: Render ile ilgili fonksiyonlar

void Window::Render()
{
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window))
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                glfwSetWindowShouldClose(window, true);
                break;
            case SDL_KEYDOWN:
                keyPresses[event.key.keysym.sym] = SDL_KEYDOWN;
                inputOnCurrentFrame = true;
                break;
            case SDL_KEYUP:
                keyPresses[event.key.keysym.sym] = SDL_KEYUP;
                inputOnCurrentFrame = true;
                break;
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                inputOnCurrentFrame = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseButtons[event.button.button] = SDL_KEYDOWN;
                inputOnCurrentFrame = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseButtons[event.button.button] = SDL_KEYUP;
                inputOnCurrentFrame = true;
                break;
            case SDL_MOUSEWHEEL:
                scrollY = event.wheel.y;
                inputOnCurrentFrame = true;
                break;
            }
        }

        for (auto buffer : buffers)
        {
            GLuint VAO = buffer.GetVAO();
            if (beforeRender.find(VAO) != beforeRender.end())
                beforeRender[VAO]();
            buffer.Bind();
            buffer.Render();
            if (afterRender.find(VAO) != afterRender.end())
                afterRender[VAO]();
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

#endif