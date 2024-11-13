#include "Helpers.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
float *CreateGridVertices(float length, int segmentation, int &size, bool onX, bool onY, bool onZ)
{
    if (segmentation < 1 || (!onX && !onY && !onZ))
    {
        return nullptr;
    }
    size = (segmentation + 2) * 24 * ((onX ? 1 : 0) + (onY ? 1 : 0) + (onZ ? 1 : 0));
    float *vertices = new float[size];
    float step = length / (segmentation + 1);

    int i = 0;
    if (onX)
        for (float x = -length / 2; x <= length / 2; x += step)
        {
            vertices[i++] = 0.0f;
            vertices[i++] = x;
            vertices[i++] = -length / 2;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;

            vertices[i++] = 0.0f;
            vertices[i++] = x;
            vertices[i++] = length / 2;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;

            vertices[i++] = 0.0f;
            vertices[i++] = -length / 2;
            vertices[i++] = x;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;

            vertices[i++] = 0.0f;
            vertices[i++] = length / 2;
            vertices[i++] = x;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
        }

    if (onY)
        for (float y = -length / 2; y <= length / 2; y += step)
        {
            vertices[i++] = -length / 2;
            vertices[i++] = 0.0f;
            vertices[i++] = y;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;

            vertices[i++] = length / 2;
            vertices[i++] = 0.0f;
            vertices[i++] = y;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;

            vertices[i++] = y;
            vertices[i++] = 0.0f;
            vertices[i++] = -length / 2;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;

            vertices[i++] = y;
            vertices[i++] = 0.0f;
            vertices[i++] = length / 2;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
            vertices[i++] = 0.0f;
        }

    if (onZ)
        for (float z = -length / 2; z <= length / 2; z += step)
        {
            vertices[i++] = -length / 2;
            vertices[i++] = z;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;

            vertices[i++] = length / 2;
            vertices[i++] = z;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;

            vertices[i++] = z;
            vertices[i++] = -length / 2;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;

            vertices[i++] = z;
            vertices[i++] = length / 2;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 0.0f;
            vertices[i++] = 1.0f;
        }

    return vertices;
}
int *CreateGridIndices(int segmentation, int &size, bool onX, bool onY, bool onZ)
{
    size = 4 * (segmentation + 2) * ((onX ? 1 : 0) + (onY ? 1 : 0) + (onZ ? 1 : 0));
    int *indices = new int[size];
    for (int i = 0; i < size; i++)
    {
        indices[i] = i;
    }
    return indices;
}

void PrepareGrid(GLuint &vertexArray, float *vertices, int vertexSize, int *indices, int indexSize)
{
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(float), vertices, GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}