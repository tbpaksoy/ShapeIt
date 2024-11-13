#include "Operator.h"
#include <glm/geometric.hpp>
#include <map>
#include <tuple>
#include <vector>
#include <algorithm>
#include <string>

static std::string GetString(Edge edge)
{
    float a = std::get<0>(edge).x,
          b = std::get<0>(edge).y,
          c = std::get<0>(edge).z,
          d = std::get<1>(edge).x,
          e = std::get<1>(edge).y,
          f = std::get<1>(edge).z;

    return std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + " -> " + std::to_string(d) + " " + std::to_string(e) + " " + std::to_string(f);
}
static std::string GetString(Triangle triangle)
{
    float a = std::get<0>(triangle).x,
          b = std::get<0>(triangle).y,
          c = std::get<0>(triangle).z,
          d = std::get<1>(triangle).x,
          e = std::get<1>(triangle).y,
          f = std::get<1>(triangle).z,
          g = std::get<2>(triangle).x,
          h = std::get<2>(triangle).y,
          i = std::get<2>(triangle).z;

    return std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + " -> " + std::to_string(d) + " " + std::to_string(e) + " " + std::to_string(f) + " -> " + std::to_string(g) + " " + std::to_string(h) + " " + std::to_string(i);
}

extern std::map<const char *, std::vector<VertexData>> OperationSpace;
float *GetOperatedValueData(int &size)
{
    std::vector<VertexData> &position = OperationSpace["position"], &colors = OperationSpace["colors"];
    size = position.size() + colors.size();
    float *data = new float[size];
    for (int i = 0; i < size; i += 6)
    {
        data[i] = position[i].value;
        data[i + 1] = position[i + 1].value;
        data[i + 2] = position[i + 2].value;
        data[i + 3] = colors[i].value;
        data[i + 4] = colors[i + 1].value;
        data[i + 5] = colors[i + 2].value;
    }
    return data;
}
int *GetOperatedIndexData(int &size)
{
    std::vector<VertexData> &indices = OperationSpace["indices"];
    size = indices.size();
    int *data = new int[size];
    for (int i = 0; i < size; i++)
        data[i] = indices[i].index;
    return data;
}
static glm::vec3 CalculateTriangleNormal(int triangle[3])
{
    glm::vec3 _triangle[3] =
        {
            glm::vec3(triangle[0], triangle[0] + 1, triangle[0] + 2),
            glm::vec3(triangle[1], triangle[1] + 1, triangle[1] + 2),
            glm::vec3(triangle[2], triangle[2] + 1, triangle[2] + 2)};
    glm::vec3 normal = glm::cross(_triangle[1] - _triangle[0], _triangle[2] - _triangle[0]);
    normal = glm::normalize(normal);
    return normal;
}
static glm::vec3 CalculateTriangleNormal(glm::vec3 triangle[3])
{
    glm::vec3 normal = glm::cross(triangle[1] - triangle[0], triangle[2] - triangle[0]);
    normal = glm::normalize(normal);
    return normal;
}
static glm::vec3 CalculateTriangleNormal(Triangle triangle)
{
    glm::vec3 normal = glm::cross(std::get<1>(triangle) - std::get<0>(triangle), std::get<2>(triangle) - std::get<0>(triangle));
    normal = glm::normalize(normal);
    return normal;
}
static int *DetermineBoundaryPoints(int *input, int inputSize, int &outputSize)
{
    if (inputSize % 3 != 0)
        return nullptr;

    std::vector<VertexData> position = OperationSpace["position"];

    std::map<std::string, int> edges;
    std::map<std::string, std::tuple<int, int>> correspondingEdges;

    for (int i = 0; i < inputSize; i += 3)
    {
        int index = input[i];
        glm::vec3 a = glm::vec3(position[index].value, position[index + 2].value, position[index + 3].value);
        index = input[i + 1];
        glm::vec3 b = glm::vec3(position[index].value, position[index + 2].value, position[index + 3].value);
        index = input[i + 2];
        glm::vec3 c = glm::vec3(position[index].value, position[index + 2].value, position[index + 3].value);

        Edge edge1 = std::make_tuple(glm::min(a, b), glm::max(a, b)),
             edge2 = std::make_tuple(glm::min(b, c), glm::max(b, c)),
             edge3 = std::make_tuple(glm::min(c, a), glm::max(c, a));

        if (correspondingEdges.find(GetString(edge1)) == correspondingEdges.end())
            correspondingEdges[GetString(edge1)] = std::make_tuple(input[i], input[i + 1]);
        if (correspondingEdges.find(GetString(edge2)) == correspondingEdges.end())
            correspondingEdges[GetString(edge2)] = std::make_tuple(input[i + 1], input[i + 2]);
        if (correspondingEdges.find(GetString(edge3)) == correspondingEdges.end())
            correspondingEdges[GetString(edge3)] = std::make_tuple(input[i + 2], input[i]);

        if (edges.find(GetString(edge1)) == edges.end())
            edges[GetString(edge1)] = 1;
        else
            edges[GetString(edge1)]++;
        if (edges.find(GetString(edge2)) == edges.end())
            edges[GetString(edge2)] = 1;
        else
            edges[GetString(edge2)]++;
        if (edges.find(GetString(edge3)) == edges.end())
            edges[GetString(edge3)] = 1;
        else
            edges[GetString(edge3)]++;
    }

    std::vector<int> indices;
    for (auto edge : edges)
    {
        if (edge.second == 1)
        {
            int a = std::get<0>(correspondingEdges[edge.first]);
            int b = std::get<1>(correspondingEdges[edge.first]);
            indices.push_back(a);
            indices.push_back(b);
        }
    }

    outputSize = indices.size();
    int *output = new int[outputSize];
    for (int i = 0; i < outputSize; i++)
        output[i] = indices[i];

    return output;
}

void Extrude(int *selectedIndices, int selectedSize, float distance)
{
    // TEST this function. (15.10.2024)
    if (selectedSize % 3 != 0)
        return;

    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < selectedSize; i += 3)
    {
        int indices[3] = {selectedIndices[i], selectedIndices[i + 1], selectedIndices[i + 2]};
        normal += CalculateTriangleNormal(indices);
    }

    normal = glm::normalize(normal);

    int boundarySize;
    int *boundary = DetermineBoundaryPoints(selectedIndices, selectedSize, boundarySize);
    glm::vec3 direction = normal * distance;
    std::vector<glm::vec3> newVertices;
    std::vector<VertexData> &position = OperationSpace["position"], &indices = OperationSpace["indices"];

    for (int i = 0; i < selectedSize; i++)
    {
        int index = selectedIndices[i];
        glm::vec3 vertex = glm::vec3(position[index].value, position[index + 1].value, position[index + 2].value);
        newVertices.push_back(vertex + direction);
    }

    std::vector<int> toDelete;
    for (int i = 0; i < selectedSize; i++)
    {
        int index = selectedIndices[i];
        if (std::find(boundary, boundary + boundarySize, index) == boundary + boundarySize)
            toDelete.push_back(index);
    }
    for (int i = 0; i < indices.size(); i += 3)
    {
        bool a = std::find(toDelete.begin(), toDelete.end(), indices[i].index) != toDelete.end(),
             b = std::find(toDelete.begin(), toDelete.end(), indices[i + 1].index) != toDelete.end(),
             c = std::find(toDelete.begin(), toDelete.end(), indices[i + 2].index) != toDelete.end();
        if (a || b || c)
        {
            indices.erase(indices.begin() + i, indices.begin() + i + 3);
            i -= 3;
        }
    }
    for (int i = 0; i < newVertices.size(); i++)
    {
        glm::vec3 vertex = newVertices[i];
        VertexData x, y, z;
        x.value = vertex.x;
        y.value = vertex.y;
        z.value = vertex.z;
        position.push_back(x);
        position.push_back(y);
        position.push_back(z);
    }

    int maxIndex = (*std::max_element(indices.begin(), indices.end(), [](VertexData a, VertexData b)
                                      { return a.index < b.index; }))
                       .index;
    // En: Extruded faces.
    // Tr: Dışa doğru çıkartılmış yüzler.

    std::vector<int> newIndices;
    for (int i = 0; i < selectedSize; i += 3)
    {
        VertexData a, b, c;
        a.index = maxIndex + i;
        b.index = maxIndex + i + 1;
        c.index = maxIndex + i + 2;

        glm::vec3 _a = newVertices[i],
                  _b = newVertices[i + 1],
                  _c = newVertices[i + 2];

        float tsp = glm::dot(_a, glm::cross(_b, _c));
        if (tsp < 0)
        {
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);

            newIndices.push_back(a.index);
            newIndices.push_back(b.index);
            newIndices.push_back(c.index);
        }
        else
        {
            indices.push_back(c);
            indices.push_back(b);
            indices.push_back(a);

            newIndices.push_back(c.index);
            newIndices.push_back(b.index);
            newIndices.push_back(a.index);
        }
    }

    // En: sides of the extruded faces.
    // Tr: Dışa doğru çıkartılmış yanları.

    int *newBoundary = DetermineBoundaryPoints(newIndices.data(), newIndices.size(), boundarySize);

    for (int i = 0; i < boundarySize; i++)
    {
        VertexData a, b, c, d;
        a.index = boundary[i];
        b.index = boundary[i + 1];
        c.index = newBoundary[i];
        d.index = newBoundary[i + 1];

        glm::vec3 _a = glm::vec3(position[a.index].value, position[a.index + 1].value, position[a.index + 2].value),
                  _b = glm::vec3(position[b.index].value, position[b.index + 1].value, position[b.index + 2].value),
                  _c = glm::vec3(position[c.index].value, position[c.index + 1].value, position[c.index + 2].value),
                  _d = glm::vec3(position[d.index].value, position[d.index + 1].value, position[d.index + 2].value);

        float tsp = glm::dot(_a, glm::cross(_b, _c));
        if (tsp < 0)
        {
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);

            indices.push_back(b);
            indices.push_back(d);
            indices.push_back(c);
        }
        else
        {
            indices.push_back(c);
            indices.push_back(b);
            indices.push_back(a);

            indices.push_back(c);
            indices.push_back(d);
            indices.push_back(b);
        }
    }

    delete[] boundary;
    delete[] newBoundary;
}