// En : This file contains class and function prototypes for object model.
// Tr : Bu dosya, nesne modeli için sınıf ve fonksiyon prototiplerini içerir.
#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <tuple>
#include <functional>
#include <vector>

// En : These are the global variables that will be used in the program.
// Tr : Bu, programda kullanılacak olan global değişkenlerdir.
float *GlobalData;
int GlobalDataElementSize;
int LastIndex = 0;
int Interval = 11;
class Object;
std::vector<Object *> GlobalObejcts;

#ifdef ADVANCED
#include "Shader.h"
enum CenterPreferece
{
    CenterOfVertices,
    CustomDefinded,
    BoundingBoxCenter
}
#endif

// En : Class declaration for the object model.
// Tr : Nesne modeli için sınıf bildirimi.
class Object
{

    /*TODO: Add optional individual shader for the object.*/
private:
#ifdef ADVANCED
    std::string name;
    glm::vec3 position, rotation, scale;
    Object *parent;
    std::vector<Object *> children;
#endif
    int positionIndex = 0, colorIndex = 3, normalIndex = 6, uvIndex = 9;
    int begin, end;

public:
    // En : Constructor and desconstructor for the object model.
    // Tr : Nesne modeli için yapıcı ve yıkıcı.
    Object(int begin, int end);
    ~Object();
#ifdef ADVANCED
    Object(int begin, int end, std::string name = nullptr, Object *parent = nullptr);
#endif

#ifdef ADVANCED
    CenterPreferece centerPreference;
#endif

    // En : Get the begin and end indices of the object.
    // Tr : Nesnenin başlangıç ve bitiş indislerini al.
    int GetBegin() const;
    int GetEnd() const;

    // En : Get the position, color, normal and uv values of the object.
    // Tr : Nesnenin pozisyon, renk, normal ve uv değerlerini al.
    glm::vec3 GetPosition(int index) const;
    glm::vec3 GetColor(int index) const;
    glm::vec3 GetNormal(int index) const;
#ifdef ADVANCED
    glm::vec3 GetCenter() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;
#endif
    glm::vec2 GetUV(int index) const;
    void Move(glm::vec3 value) noexcept;
    void Rotate(glm::vec3 value) noexcept;
    void Scale(glm::vec3 value) noexcept;
#ifdef ADVANCED
    void CalculateBoundingBox(glm::vec3 &min, glm::vec3 &max);
    std::string GetName() const;
    Object *GetParent() const;
    std::vector<Object *> GetChildren() const;
#endif
};
#endif