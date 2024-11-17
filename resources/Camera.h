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
    float fov = 45.0f, aspectRatio = 800.0f / 600.0f, nearView = 0.1f, farView = 100.0f;

public:
    Camera();
    Camera(glm::vec3 position, glm::quat rotation, glm::vec3 scale, ProjectionType projectionType = Perspective, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
    ~Camera();
    ProjectionType projectionType;

    // En: Directions.
    // Tr: Yönler.

    glm::vec3 Front() const;
    glm::vec3 Up() const;
    glm::vec3 Right() const;

    // En: Related to rendering.
    // Tr: Render ile ilgili.

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetModelMatrix() const;
    float GetAspectRatio() const;
    float GetFov() const;
    float GetNear() const;
    float GetFar() const;
    void SetAspectRatio(float aspectRatio);
    void SetFov(float fov);
    void SetNear(float near);
    void SetFar(float far);

    // En: Transforms.
    // Tr: Dönüşümler.

    void Move(glm::vec3 offset);
    void Rotate(glm::quat rotation);
    void Scale(glm::vec3 scale);
    void LookAt(glm::vec3 target);
};
#endif