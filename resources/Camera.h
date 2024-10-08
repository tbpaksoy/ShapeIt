#include <glm/glm.hpp>

enum ProjectionType
{
    Perspective,
    Orthographic
};

#ifndef CAMERA_H
#define CAMERA_H
class Camera
{
private:
    glm::vec3 position, rotation, scale;
    glm::mat4 viewMatrix, projectionMatrix;
    glm::vec3 up;

public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, ProjectionType projectionType = Perspective, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
    ~Camera();
    ProjectionType projectionType;
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void Move(glm::vec3 offset);
    void Rotate(glm::vec3 rotation);
    void Scale(glm::vec3 scale);
};
#endif