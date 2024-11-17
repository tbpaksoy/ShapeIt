#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <functional>
#include "Buffer.h"
#include "Shader.h"
#ifndef WINDOW_H
#define WINDOW_H
class Window;
void Render(Window **windows, int count);

#ifdef IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#else
#include <GL/glew.h>
#endif

class Window
{
private:
    // En: Window related variables
    // Tr: Pencere ile ilgili değişkenler

    GLFWwindow *window;
    int width, height;
    const char *title;
    bool resizable;
    float lastFrame = 0.0f, currentFrame = 0.0f, deltaTime = 0.0f;

    // En: Rendering related variables
    // Tr: Render ile ilgili değişkenler

    std::vector<Buffer> buffers;
#ifdef IMGUI
    std::map<ImU32, std::function<void()>> beforeRender, afterRender;
    std::map<ImU32, Shader *> shaders;
    std::vector<std::function<void()>> imguiRender;
#else
    std::map<GLuint, std::function<void()>> beforeRender, afterRender;
    std::map<GLuint, Shader *> shaders;
#endif
    std::vector<std::function<void()>> onFrame;

    // En: Input related variables
    // Tr: Girdi ile ilgili değişkenler

    std::map<int, int> mouseButtons, keyPresses;
    double mouseX, mouseY, scrollY;
    bool inputOnCurrentFrame;

public:
    // En: Constructor and destructor
    // Tr: Yapıcı ve yıkıcı fonksiyonlar

    Window(const char *title, int width = 0, int height = 0, GLFWmonitor *monitor = nullptr,
           bool resizable = false);
    ~Window();

    // En: Functions to get and update window related variables
    // Tr: Pencere ile ilgili değişkenleri almak ve güncellemek için fonksiyonlar

    const char *GetTitle() const;
    int GetWidth() const;
    int GetHeight() const;
    void ScreenSize(int &width, int &height);
    void UpdateTitle(const char *title);
    void UpdateResizable(bool resizable);
    void SetFullscreen();
    void SetFullscreen(GLFWmonitor *monitor);
    void AddOnFrame(std::function<void()> onFrame);
    float GetDeltaTime() const;

    // En: Functions related to input
    // Tr: Girdi ile ilgili fonksiyonlar

    bool MouseButton(int button, int &action);
    bool KeyPress(int key, int &action);
    void MousePosition(float &x, float &y);
    void MouseScroll(int &y);

    // En: Functions related to rendering
    // Tr: Render ile ilgili fonksiyonlar

#ifdef IMGUI
    void AddImGuiRender(std::function<void()> imguiRender);
    void ClearImGuiRender();
#endif
    void Render();
    void AddBuffer(Buffer buffer);

    friend void Render(Window **windows, int count);
};
#endif