#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#ifndef CAMERA_H
#define CAMERA_H

enum ProjectionType
{
    Perspective,
    Orthographic
};

class Camera
{
private:
    glm::vec3 position, scale;
    glm::quat rotation;
    mutable glm::mat4 viewMatrix, projectionMatrix, modelMatrix;
    glm::vec3 up;

public:
    Camera();
    Camera(glm::vec3 position, glm::quat rotation, glm::vec3 scale, ProjectionType projectionType = Perspective, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
    ~Camera();
    ProjectionType projectionType;
    glm::vec3 Front() const;
    glm::vec3 Up() const;
    glm::vec3 Right() const;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetModelMatrix() const;
    void Move(glm::vec3 offset);
    void Rotate(glm::quat rotation);
    void Scale(glm::vec3 scale);
    void LookAt(glm::vec3 target);
};
#endif