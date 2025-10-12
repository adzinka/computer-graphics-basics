// Camera.cpp
#include "Camera.h"
#include "ShaderProgram.h" 

Camera::Camera()
    : position_(0.0f, 0.0f, 5.0f),       // Stojíme na pozici Z=5, díváme se do středu
    worldUp_(0.0f, 1.0f, 0.0f),      // Světová osa Y je "nahoře"
    yaw_(-90.0f),                      // Natočení 0 stupňů je podél kladné osy X, -90 nás otočí směrem k záporné ose Z
    pitch_(0.0f),
    fov_(45.0f),
    aspectRatio_(16.0f / 9.0f),        // Výchozí poměr stran
    nearPlane_(0.1f),
    farPlane_(100.0f)
{
    // Na začátku musíme inicializovat směrové vektory
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    // Vytvoří pohledovou matici z pozice, cíle (pozice + směr) a "up" vektoru
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::mat4 Camera::getProjectionMatrix() const {
    // Vytvoří perspektivní projekční matici
    return glm::perspective(glm::radians(fov_), aspectRatio_, nearPlane_, farPlane_);
}

void Camera::onMouseMove(float xoffset, float yoffset) {
    float sensitivity = 0.1f; // Nastavení citlivosti myši
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // Omezení úhlu pitch, abychom se nemohli podívat "za sebe" nahoru nebo dolů
    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    // Po změně úhlů musíme přepočítat všechny směrové vektory
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Výpočet nového vektoru "front" z Eulerových úhlů
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    // Přepočet vektorů "right" a "up"
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

void Camera::addObserver(ShaderProgram* observer) {
    observers_.push_back(observer);
}

void Camera::removeObserver(ShaderProgram* observer) {
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

    // Po změně pozice musíme informovat pozorovatele (pokud používáte Observer)
    // nebo se změna projeví v příštím volání getViewMatrix()
    notify(); // Pokud máte Observer, jinak tento řádek není potřeba
}

void Camera::notify() const {
    for (ShaderProgram* observer : observers_) {
        if (observer) {
            observer->update(*this);
        }
    }
}

