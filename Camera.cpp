#include "Camera.hpp"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Camera::Camera() {
    updatePVMat();
}

void Camera::updatePVMat() {
    glm::mat4 projection;
    switch (type) {
        case PERSPECTIVE:
            projection = glm::perspective(glm::radians(fovY), aspect, nf[0], nf[1]);
            break;
        case ORTHOGRAPHIC:
            projection = glm::ortho(lr[0], lr[1], bt[0], bt[1], nf[0], nf[1]);
            break;
    }
    glm::mat4 view = glm::lookAt(eye, eye + dir, up);
    pvMat = projection * view;
}

const glm::mat4 &Camera::getPVMat() {
    return pvMat;
}

CameraType Camera::getType() const {
    return type;
}

void Camera::setType(CameraType type) {
    Camera::type = type;
    updatePVMat();
}

const glm::vec3 &Camera::getEye() const {
    return eye;
}

void Camera::setEye(const glm::vec3 &eye) {
    Camera::eye = eye;
    updatePVMat();
}

const glm::vec3 &Camera::getRot() const {
    return rot;
}

void Camera::setRot(const glm::vec3 &rot) {
    Camera::rot = rot;
    // Y -> Z -> X
    dir = glm::rotate(rot.x, glm::vec3(1, 0, 0)) * glm::rotate(rot.z, glm::vec3(0, 0, 1)) * glm::rotate(rot.y, glm::vec3(0, 1, 0)) * glm::vec4(Camera::defaultDir, 0);
    updatePVMat();
}

const glm::vec3 &Camera::getUp() const {
    return up;
}

void Camera::setUp(const glm::vec3 &up) {
    Camera::up = up;
    updatePVMat();
}

const glm::vec2 &Camera::getNf() const {
    return nf;
}

void Camera::setNf(const glm::vec2 &nf) {
    Camera::nf = nf;
    updatePVMat();
}

float Camera::getFovY() const {
    return fovY;
}

void Camera::setFovY(float fovY) {
    Camera::fovY = fovY;
    updatePVMat();
}

float Camera::getAspect() const {
    return aspect;
}

void Camera::setAspect(float aspect) {
    Camera::aspect = aspect;
    updatePVMat();
}

const glm::vec2 &Camera::getLr() const {
    return lr;
}

void Camera::setLr(const glm::vec2 &lr) {
    Camera::lr = lr;
    updatePVMat();
}

const glm::vec2 &Camera::getBt() const {
    return bt;
}

void Camera::setBt(const glm::vec2 &bt) {
    Camera::bt = bt;
    updatePVMat();
}

const glm::vec3 &Camera::getDir() const {
    return dir;
}

void Camera::setDir(const glm::vec3 &dir) {
    Camera::dir = dir;
    // compute rotation
    rot = {
            glm::orientedAngle(dir, defaultDir, glm::vec3(1, 0, 0)),
            glm::orientedAngle(dir, defaultDir, glm::vec3(0, 1, 0)),
            glm::orientedAngle(dir, defaultDir, glm::vec3(0, 0, 1))
    };
    updatePVMat();
}
