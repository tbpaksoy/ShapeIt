#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

// FIXME: Camea class is not working properly. Movement does not work as expected. (19.10.2024)
Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    projectionType = Perspective;
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}
Camera::Camera(glm::vec3 position, glm::quat rotation, glm::vec3 scale, ProjectionType projectionType, glm::vec3 up)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->projectionType = projectionType;
    this->up = up;
}
Camera::~Camera()
{
}
glm::vec3 Camera::Front() const
{
    return rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}
glm::vec3 Camera::Up() const
{
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}
glm::vec3 Camera::Right() const
{
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}
glm::mat4 Camera::GetViewMatrix() const
{
    std::cout << glm::to_string(Front()) << std::endl;
    viewMatrix = glm::lookAt(position, position + Front(), Up());
    return viewMatrix;
}
glm::mat4 Camera::GetProjectionMatrix() const
{
    switch (projectionType)
    {
    case Perspective:
        projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.01f, 100.0f);
        break;
    case Orthographic:
        projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        break;
    }
    return projectionMatrix;
}
glm::mat4 Camera::GetModelMatrix() const
{
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), position),
              rotation = glm::mat4_cast(this->rotation),
              scale = glm::scale(glm::mat4(1.0f), this->scale);
    modelMatrix = translate * rotation * scale;
    return modelMatrix;
}
void Camera::Move(glm::vec3 offset)
{
    position += offset;
}
void Camera::Rotate(glm::quat rotation)
{
    this->rotation = glm::normalize(rotation + this->rotation);
}
void Camera::Scale(glm::vec3 scale)
{
    this->scale += scale;
}
void Camera::LookAt(glm::vec3 target)
{
    glm::vec3 direction = glm::normalize(target - position);
    glm::quat lookAt = glm::rotation(glm::vec3(0.0f, 0.0f, 1.0f), direction);
    rotation = lookAt;
}