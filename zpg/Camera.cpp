#include "Camera.h"

Camera::Camera()
    : position_(0.0f, 2.0f, 5.0f),       
    worldUp_(0.0f, 1.0f, 0.0f),      
    yaw_(-90.0f),                      
    pitch_(-30.0f),
    fov_(45.0f),
    aspectRatio_(800.0f / 600.0f),        
    nearPlane_(0.1f),
    farPlane_(100.0f)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov_), aspectRatio_, nearPlane_, farPlane_);
}

void Camera::onMouseMove(float xoffset, float yoffset) {
    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw_ += xoffset;
    pitch_ += yoffset;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, worldUp_));
    up_ = glm::normalize(glm::cross(right_, front_));
    notify();
}

void Camera::onWindowResize(float width, float height) {
    if (height > 0) {
        aspectRatio_ = width / height;
    }
    notify();
}

void Camera::addObserver(ICameraObserver* observer) {
    observers_.push_back(observer);
    observer->updateCamera(*this);
}

void Camera::removeObserver(ICameraObserver* observer) {
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
}

void Camera::move(Camera_Movement direction, float deltaTime) {
    float velocity = 2.5f * deltaTime;
    if (direction == FORWARD)
        position_ += front_ * velocity;
    if (direction == BACKWARD)
        position_ -= front_ * velocity;
    if (direction == LEFT)
        position_ -= right_ * velocity;
    if (direction == RIGHT)
        position_ += right_ * velocity;

    notify();
}

void Camera::notify() const {
    for (ICameraObserver* observer : observers_) {
        if (observer) {
            observer->updateCamera(*this);
        }
    }
}

void Camera::setPosition(const glm::vec3& pos) {
    position_ = pos;

    notify();
}

void Camera::setFov(float fov) {
    fov_ = fov; 

    notify();
}