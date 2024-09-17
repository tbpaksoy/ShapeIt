#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef XML
#define XML
#include "tinyxml2.h"
#include "tinyxml2.cpp"
#endif

#include "resources/Shader.h"
#include "resources/Shader.cpp"

// #include "resources/Object.h"
#ifdef OBJECT_H
#include "resources/Object.cpp"
#endif

#include "resources/Analyze.cpp"
#ifdef ANALYZE_H
#include "resources/Analyze.h"
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include "resources/Text.h"
#ifdef TEXT_H
#include "resources/Text.cpp"
#endif

std::map<std::string, GLuint> attributeLocations{
    {"position", 0},
    {"color", 1},
    {"normal", 2},
    {"uv", 3}};
std::map<std::string, GLuint> attributeSizes{
    {"position", 3},
    {"color", 3},
    {"normal", 3},
    {"uv", 2}};

void ErrorCallback(int error, const char *description)
{
    std::cout << "Error: " << description << std::endl;
}

void ErrorDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::cout << "Error: " << message << std::endl;
}

void PrepareAndLoadMeshData(MeshData data, GLuint &vertexArray, int &size)
{
    if (data.find("position") == data.end() || data.find("index") == data.end() || data.find("color") == data.end())
    {
        std::cout << "Data is not complete\n";
        return;
    }

    std::vector<int> indices = std::get<std::vector<int>>(data["index"]);
    size = indices.size();
    int refSize = std::get<std::vector<glm::vec3>>(data["position"]).size();
    std::map<std::string, int> offsets{
        {"position", 0},
        {"color", 3}};

    int interval = 6;

    if (data.find("normal") != data.end())
    {
        interval += 3;
        offsets["normal"] = interval;
    }
    if (data.find("uv") != data.end())
    {
        interval += 2;
        offsets["uv"] = interval;
    }

#ifdef OBJECT_H
    Interval = interval;
#endif

    float *originalData = new float[refSize * interval];

    for (std::map<std::string, int>::iterator it = offsets.begin(); it != offsets.end(); ++it)
    {
        int offset = (*it).second;
        std::string key = (*it).first;
        if (std::holds_alternative<std::vector<glm::vec2>>(data[key]))
        {
            std::vector<glm::vec2> v2s = std::get<std::vector<glm::vec2>>(data[key]);
            for (int i = 0; i < refSize; i++)
            {
                glm::vec2 v2 = v2s[i];

                originalData[i * interval + offset] = v2.x;
                originalData[i * interval + offset + 1] = v2.y;
            }
        }
        else if (std::holds_alternative<std::vector<glm::vec3>>(data[key]))
        {
            std::vector<glm::vec3> v3s = std::get<std::vector<glm::vec3>>(data[key]);
            for (int i = 0; i < refSize; i++)
            {
                glm::vec3 v3 = v3s[i];

                originalData[i * interval + offset] = v3.x;
                originalData[i * interval + offset + 1] = v3.y;
                originalData[i * interval + offset + 2] = v3.z;
            }
        }
    }

#ifdef OBJECT_H
    GlobalData = originalData;
    GlobalDataElementSize = refSize * interval;
#endif

    GLuint vertexBuffer, elementBuffer;

    glGenBuffers(1, &elementBuffer);
    glGenBuffers(1, &vertexBuffer);
    glGenVertexArrays(1, &vertexArray);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    glBufferData(GL_ARRAY_BUFFER, refSize * interval * sizeof(float), originalData, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(int), &indices.front(), GL_STATIC_DRAW);

    for (std::map<std::string, int>::iterator it = offsets.begin(); it != offsets.end(); ++it)
    {
        try
        {
            GLuint location = attributeLocations[(*it).first];
            GLuint size = attributeSizes[(*it).first];
            glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, interval * sizeof(float), (void *)((*it).second * sizeof(float)));
            glEnableVertexAttribArray(location);
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << (*it).first << "->" << (*it).second << std::endl;
        }
    }
}

#ifdef TEXT_H
void PrepareTextBuffer(std::vector<Text *> texts, GLuint &vertexArray, int &size)
{
    int _size = 0;
    for (Text *t : texts)
    {
        _size += t->GetQuadCount() * 24;
    }
    float *data = new float[_size];

    for (int i = 0, j = 0; j < _size; i++)
    {
        Text *text = texts[i];
        int count;
        float *vertices = text->GetVertices(count);
        for (int k = j; k < j + count; k++)
            data[k] = vertices[k - j];
        j += count;
    }

    _size = 0;
    for (Text *t : texts)
    {
        _size += t->GetQuadCount();
    }
    GLuint *textures = new GLuint[_size];
    for (int i = 0, j = 0; j < _size; i++)
    {
        int count;
        GLuint *tex = texts[i]->GetTextures(count);
        for (int k = j; k < j + count; k++)
            textures[k] = tex[k - j];
        j += count;
    }

    float *verts = new float[24];
    for (int i = 0; i < 24; i++)
        verts[i] = data[i];

    size = 6;

    GLuint vertexBuffer;

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts) * sizeof(float), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}
#endif

int main()
{

    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(800, 800, "ShapeIt", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Glew could not be initialized\n";
        std::cout << glewGetErrorString(err);
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetErrorCallback(ErrorCallback);
    glDebugMessageCallback(ErrorDebugCallback, nullptr);

    Shader *meshShader = new Shader("Shaders\\vertex.vs", "Shaders\\fragment.fs");

    tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
    doc->LoadFile("test.xml");
    MeshData md;
    AnalyzeTag(doc->RootElement(), md);
    GLuint meshVertexArray;
    int meshSize;
    PrepareAndLoadMeshData(md, meshVertexArray, meshSize);

#ifdef OBJECT_H
    float prevTime = 0, currentTime = 0;
#endif

#ifdef TEXT_H

    FT_Init_FreeType(&ft);

    Shader *textShader = new Shader("Shaders\\text.vs", "Shaders\\text.fs");

    textShader->SetUniform(std::string("projection").c_str(), glm::ortho(0.0f, 800.0f, 0.0f, 800.0f));

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error: " << error << std::endl;
    }

    Text *text = new Text("test", "fonts\\unifont-15.1.05.otf");

    int size;
    GLuint *textures = text->GetTextures(size);

    for (int i = 0; i < size; i++)
    {
        std::cout << textures[i] << std::endl;
    }

    GLuint textVertexArray;
    int textSize;
    PrepareTextBuffer({text}, textVertexArray, textSize);

#endif

    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

#ifdef OBJECT_H
        currentTime = glfwGetTime();
        for (Object *o : GlobalObejcts)
        {
            // o->Rotate(glm::vec3(0.35f, 0.35f, 0.35f) * (currentTime - prevTime) * 20.0f);
        }
        glBufferData(GL_ARRAY_BUFFER, GlobalDataElementSize * sizeof(float), GlobalData, GL_STATIC_DRAW);
        prevTime = currentTime;
#endif

        meshShader->Use();
        glBindVertexArray(meshVertexArray);
        glDrawElements(GL_TRIANGLES, meshSize, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

#ifdef TEXT_H
        textShader->Use();
        glBindVertexArray(textVertexArray);
        glDrawArrays(GL_TRIANGLES, 0, textSize * 24);
#endif
    }

#ifdef TEXT_H
    FT_Done_FreeType(ft);
#endif
}