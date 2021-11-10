#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraType {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Camera {
public:
    Camera();
    const glm::mat4 &getPVMat();
    CameraType getType() const;
    void setType(CameraType type);
    const glm::vec3 &getEye() const;
    void setEye(const glm::vec3 &eye);
    const glm::vec3 &getRot() const;
    void setRot(const glm::vec3 &rot);
    const glm::vec3 &getUp() const;
    void setUp(const glm::vec3 &up);
    const glm::vec3 &getDir() const;
    void setDir(const glm::vec3 &dir);
    const glm::vec2 &getNf() const;
    void setNf(const glm::vec2 &nf);
    float getFovY() const;
    void setFovY(float fovY);
    float getAspect() const;
    void setAspect(float aspect);
    const glm::vec2 &getLr() const;
    void setLr(const glm::vec2 &lr);
    const glm::vec2 &getBt() const;
    void setBt(const glm::vec2 &bt);

private:
    inline static constexpr glm::vec3 defaultDir{0, 0, 1};

    CameraType type = PERSPECTIVE;
    glm::mat4 pvMat = glm::mat4(0.0f);
    glm::vec3 eye{0, 0, 0};
    glm::vec3 rot{0, 0, 0};
    // center = eye + dir
    glm::vec3 dir{0, 0, 1};
    glm::vec3 up{0, 1, 0};
    glm::vec2 nf{0, 100};

    // PERSPECTIVE specific
    float fovY = 30;
    float aspect = 4.0f / 3.0f;

    // ORTHOGRAPHIC specific
    glm::vec2 lr{-10, 10};
    glm::vec2 bt{-10, 10};

    void updatePVMat();
};

#endif //CAMERA_HPP
