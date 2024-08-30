#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <tuple>
#include <functional>
#include <vector>
float *GlobalData;
int GlobalDataElementSize;
int LastIndex = 0;
int Interval = 11;
class Object;
std::vector<Object *> GlobalObejcts;
class Object
{
private:
    int positionIndex = 0, colorIndex = 3, normalIndex = 6, uvIndex = 9;
    int begin, end;

public:
    Object(int begin, int end);
    ~Object();
    glm::vec3 GetPosition(int index) const;
    glm::vec3 GetColor(int index) const;
    glm::vec3 GetNormal(int index) const;
    glm::vec2 GetUV(int index) const;
    void Move(glm::vec3 value) noexcept;
    void Rotate(glm::vec3 value) noexcept;
    void Scale(glm::vec3 value) noexcept;
};
#endif