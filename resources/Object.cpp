// En : Definitions of Analyze.h file.
// Tr : Analyze.h dosyasının tanımlamaları.
#include "Object.h"
#include <vector>
#include <numeric>
#include <glm/gtx/rotate_vector.hpp>
Object::Object(int begin, int end) : begin(begin), end(end)
{
    // En : All object must be added to the global object list.
    // Tr : Tüm nesneler global nesne listesine eklenmelidir.
    GlobalObejcts.push_back(this);
}
Object::~Object() {}
#ifdef ADVANCED
Object(int begin, int end, std::string name, Object *parent) : begin(begin), end(end), name(name), parent(parent)
{
    GlobalObejcts.push_back(this);
}
#endif
int Object::GetBegin() const
{
    return begin;
}
int Object::GetEnd() const
{
    return end;
}
glm::vec3 Object::GetPosition(int index) const
{
    int temp = index * Interval + positionIndex;
    float x = GlobalData[temp],
          y = GlobalData[temp + 1],
          z = GlobalData[temp + 2];
    return glm::vec3(x, y, z);
}
glm::vec3 Object::GetColor(int index) const
{
    int temp = index * Interval + colorIndex;
    float r = GlobalData[temp],
          g = GlobalData[temp + 1],
          b = GlobalData[temp + 2];
    return glm::vec3(r, g, b);
}
glm::vec3 Object::GetNormal(int index) const
{
    int temp = index * Interval + normalIndex;
    float x = GlobalData[temp],
          y = GlobalData[temp + 1],
          z = GlobalData[temp + 2];
    return glm::vec3(x, y, z);
}
#ifdef ADVANCED
glm::vec3 Object::GetCenter() const
{
    // TODO: Implement the center calculation. (25.09.2024)
    glm::vec3 result = glm::vec3();
    switch (centerPreference)
    {
    case CenterOfVertices:
        for (int i = begin * 3; i < end * 3; i++)
        {
            float x = GlobalData[i + positionIndex], y = GlobalData[i + positionIndex + 1], z = GlobalData[i + positionIndex + 2];
            result += glm::vec3(x, y, z);
        }
        result /= (float)(end - begin);
        break;
    case CustomDefined:
        // TODO:Implement this section.
        break;
    case BoundingBoxCenter:
        glm::vec3 min, max;
        CalculateBoundingBox(min, max);
        float x = (max.x - std::abs(min.x)) / 2,
              y = (max.y - std::abs(min.y)) / 2,
              z = (max.z - std::abs(min.z)) / 2;
        result = glm::vec3(z, y, z);
        break;
    }
    return result;
}
#endif
glm::vec2 Object::GetUV(int index) const
{
    int temp = index * Interval + uvIndex;
    float u = GlobalData[temp],
          v = GlobalData[temp + 1];
    return glm::vec2(u, v);
}
void Object::Move(glm::vec3 value) noexcept
{
    for (int i = begin * Interval; i < end * Interval; i += Interval)
    {
        GlobalData[i + positionIndex] += value.x;
        GlobalData[i + positionIndex + 1] += value.y;
        GlobalData[i + positionIndex + 2] += value.z;
    }
}
void Object::Rotate(glm::vec3 value) noexcept
{
    // En : Get the positions of the object at the defined indices.
    // Tr : Nesnenin tanımlanan indislerindeki pozisyonlarını al.
    std::vector<glm::vec3> positions;
    for (int i = begin * Interval; i < end * Interval; i += Interval)
    {
        float x = GlobalData[i + positionIndex],
              y = GlobalData[i + positionIndex + 1],
              z = GlobalData[i + positionIndex + 2];

        positions.push_back(glm::vec3(x, y, z));
    }
    // En : Get the center of the object by points.
    // Tr : Nesnenin merkezini noktalardan al.
    glm::vec3 center = std::accumulate(positions.begin(), positions.end(), glm::vec3(0)) / static_cast<float>(positions.size());
    for (glm::vec3 &position : positions)
    {
        position -= center;
        position = glm::rotateX(position, glm::radians(value.x));
        position = glm::rotateY(position, glm::radians(value.y));
        position = glm::rotateZ(position, glm::radians(value.z));
        position += center;
    }
    for (int i = begin * Interval, j = 0; i < end * Interval; i += Interval, j++)
    {
        GlobalData[i + positionIndex] = positions[j].x;
        GlobalData[i + positionIndex + 1] = positions[j].y;
        GlobalData[i + positionIndex + 2] = positions[j].z;
    }
}
void Object::Scale(glm::vec3 value) noexcept
{
    std::vector<glm::vec3> positions;
    for (int i = begin * Interval; i < end * Interval; i += Interval)
    {
        float x = GlobalData[i + positionIndex],
              y = GlobalData[i + positionIndex + 1],
              z = GlobalData[i + positionIndex + 2];
        positions.push_back(glm::vec3(x, y, z));
    }
    glm::vec3 center = std::accumulate(positions.begin(), positions.end(), glm::vec3(0)) / static_cast<float>(positions.size());
    for (glm::vec3 &position : positions)
    {
        position -= center;
        position *= value;
        position += center;
    }
    for (int i = begin * Interval, j = 0; i < end * Interval; i += Interval, j++)
    {
        GlobalData[i + positionIndex] = positions[j].x;
        GlobalData[i + positionIndex + 1] = positions[j].y;
        GlobalData[i + positionIndex + 2] = positions[j].z;
    }
}
#ifdef ADVANCED
void Object::CalculateBoundingBox(glm::vec3 &min, glm::vec3 &max)
{
    for (int i = begin * Interval; i < end * Interval; i += 3)
    {
    }
}
#endif