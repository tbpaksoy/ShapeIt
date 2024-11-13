#include <glm/glm.hpp>
#include <GL/glew.h>
#ifndef HELPERS_H
#define HELPERS_H
float *CreateGridVertices(float length, int segmentation, int &size, bool onX = false, bool onY = true, bool onZ = false);
int *CreateGridIndices(int segmentation, int &size, bool onX = false, bool onY = true, bool onZ = false);

void PrepareGrid(GLuint &vertexArray, float *vertices, int vertexSize, int *indices, int indexSize);
#endif