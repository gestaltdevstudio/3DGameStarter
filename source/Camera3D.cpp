#include "../include/Camera3D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GGE
{

Camera3D::Camera3D()
    : position(4.0f, 3.0f, -3.0f),
      target(0.0f, 0.0f, 0.0f),
      up(0.0f, 1.0f, 0.0f),
      fov(45.0f),
      aspect(16.0f / 9.0f),
      nearClip(0.1f),
      farClip(100.0f)
{
}

void Camera3D::setPosition(const glm::vec3& value)
{
    position = value;
}

void Camera3D::setTarget(const glm::vec3& value)
{
    target = value;
}

void Camera3D::setUp(const glm::vec3& value)
{
    up = value;
}

void Camera3D::setPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
{
    fov = fovDegrees;
    aspect = aspectRatio;
    nearClip = nearPlane;
    farClip = farPlane;
}

glm::mat4 Camera3D::getProjectionMatrix() const
{
    return glm::perspective(fov, aspect, nearClip, farClip);
}

glm::mat4 Camera3D::getViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

}