#include "Camera.hpp"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <cstdio>
#include <stdexcept>

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
        default:
            throw std::logic_error("invalid camera type");
    }

    // Y -> Z -> X
    glm::vec3 dir = getRotMat() * glm::vec4(Camera::defaultDir, 0);
    fprintf(stderr, "cam dir: %f %f %f\n", dir.x, dir.y, dir.z);
    glm::mat4 view = glm::lookAt(eye, eye + dir, up);

    pvMat = projection * view;
}

glm::mat4 Camera::getPVMat() {
    return pvMat;
}

CameraType Camera::getType() const {
    return type;
}

void Camera::setType(CameraType type) {
    Camera::type = type;
    updatePVMat();
}

glm::vec3 Camera::getEye() const {
    return eye;
}

void Camera::setEye(const glm::vec3 &eye) {
    Camera::eye = eye;
    updatePVMat();
}

glm::vec3 Camera::getRot() const {
    return rot;
}

void Camera::setRot(const glm::vec3 &rot) {
    Camera::rot = rot;
    updatePVMat();
}

glm::vec3 Camera::getUp() const {
    return up;
}

void Camera::setUp(const glm::vec3 &up) {
    Camera::up = up;
    updatePVMat();
}

glm::vec2 Camera::getNf() const {
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

glm::vec2 Camera::getLr() const {
    return lr;
}

void Camera::setLr(const glm::vec2 &lr) {
    Camera::lr = lr;
    updatePVMat();
}

glm::vec2 Camera::getBt() const {
    return bt;
}

void Camera::setBt(const glm::vec2 &bt) {
    Camera::bt = bt;
    updatePVMat();
}

glm::mat4 Camera::getRotMat() const {
    return glm::rotate(rot.x, glm::vec3(1, 0, 0)) * glm::rotate(rot.z, glm::vec3(0, 0, 1)) * glm::rotate(rot.y, glm::vec3(0, 1, 0));
}

void Camera::rotate(glm::vec3 axis, float a) {
    pvMat *= glm::rotate(a, axis);
}
