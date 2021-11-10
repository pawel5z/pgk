#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

enum CameraType {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

enum Space {
    SELF,
    WORLD
};

class Camera {
public:
    Camera() = default;
    Camera(float fovY, float aspect, float near, float far);
    Camera(float left, float right, float bottom, float top, float near, float far);
    glm::mat4 getPVMat();
    CameraType getType() const;
    void setType(CameraType type);
    glm::vec3 getEye() const;
    void setEye(const glm::vec3 &eye);
    glm::vec3 getRot() const;
    void setRot(const glm::vec3 &rot);
    glm::vec2 getNf() const;
    void setNf(const glm::vec2 &nf);
    float getFovY() const;
    void setFovY(float fovY);
    float getAspect() const;
    void setAspect(float aspect);
    glm::vec2 getLr() const;
    void setLr(const glm::vec2 &lr);
    glm::vec2 getBt() const;
    void setBt(const glm::vec2 &bt);
    glm::mat4 getRotMat() const;
    void rotate(glm::vec3 axis, float angle, Space space = SELF);

private:
    inline static constexpr glm::vec3 defaultDir{0, 0, 1};

    CameraType type = PERSPECTIVE;
    glm::vec3 eye = glm::vec3(0, 0, 0);
    glm::quat rot = glm::quat(); // identity
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec2 nf = glm::vec2(0.1, 100);

    // PERSPECTIVE specific
    float fovY = 30;
    float aspect = 4.0f / 3.0f;

    // ORTHOGRAPHIC specific
    glm::vec2 lr = glm::vec2(-10, 10);
    glm::vec2 bt = glm::vec2(-10, 10);
};

#endif //CAMERA_HPP
