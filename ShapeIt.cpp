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

#include "resources/Object.h"
#ifdef OBJECT_H
#include "resources/Object.cpp"
#endif

#include "resources/Analyze.cpp"
#ifdef ANALYZE_H
#include "resources/Analyze.h"
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

void PrepareAndLoadData(MeshData data, GLuint &vertexArray, int &size)
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

int main()
{

    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(800, 800, "ShapeIt", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Glew could not be initialized\n";
        std::cout << glewGetErrorString(err);
        return -1;
    }

    tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
    doc->LoadFile("test.xml");
    MeshData md;
    AnalyzeTag(doc->RootElement(), md);
    GLuint vertexArray;
    int size;
    PrepareAndLoadData(md, vertexArray, size);

    Shader *shader = new Shader("Shaders\\vertex.vs", "Shaders\\fragment.fs");

#ifdef OBJECT_H
    float prevTime = 0, currentTime = 0;
#endif
    while (!glfwWindowShouldClose(window))
    {
#ifdef OBJECT_H
        currentTime = glfwGetTime();
        for (Object *o : GlobalObejcts)
        {
            o->Rotate(glm::vec3(0, 1.0f, 0) * (currentTime - prevTime) * 10.0f);
        }
        glBufferData(GL_ARRAY_BUFFER, GlobalDataElementSize * sizeof(float), GlobalData, GL_STATIC_DRAW);
        prevTime = currentTime;
#endif
        glClear(GL_COLOR_BUFFER_BIT);
        shader->Use();
        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}