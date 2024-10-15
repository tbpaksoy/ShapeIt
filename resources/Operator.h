#include <glm/glm.hpp>
#include <vector>
#ifndef OPERATOR_H
#define OPERATOR_H

union VertexData
{
    int index;
    float value;
};

typedef std::tuple<glm::vec3, glm::vec3> Edge;
typedef std::tuple<glm::vec3, glm::vec3, glm::vec3> Triangle;

std::map<const char *, std::vector<VertexData>> OperationSpace =
    {
        {"position", std::vector<VertexData>()},
        {"normals", std::vector<VertexData>()},
        {"colors", std::vector<VertexData>()},
        {"indices", std::vector<VertexData>()}};

float *GetOperatedValueData(int &size);
int *GetOperatedIndexData(int &size);

void Extrude(int *selectedIndices, int selectedSize, float distance);
void Extrude(int *selectedIndices, int selectedSize, float distance, glm::vec3 direction);

#endif