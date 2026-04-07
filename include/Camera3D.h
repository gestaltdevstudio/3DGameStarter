#ifndef CAMERA3D_H_INCLUDED
#define CAMERA3D_H_INCLUDED

#include <glm/glm.hpp>

namespace GGE
{

class Camera3D
{
public:
    Camera3D();

    void setPosition(const glm::vec3& value);
    void setTarget(const glm::vec3& value);
    void setUp(const glm::vec3& value);
    void setPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getTarget() const { return target; }
    const glm::vec3& getUp() const { return up; }

    float getFovDegrees() const { return fov; }
    float getAspectRatio() const { return aspect; }
    float getNearPlane() const { return nearClip; }
    float getFarPlane() const { return farClip; }

    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewMatrix() const;

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float fov;
    float aspect;
    float nearClip;
    float farClip;
};

}

#endif // CAMERA3D_H_INCLUDED