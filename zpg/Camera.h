#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>

#include "ICameraObserver.h"

class Camera {
public:
    static constexpr float DEFAULT_MOVEMENT_SPEED = 2.5f;
    static constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.1f;
    static constexpr float DEFAULT_FOV = 45.0f;
    static constexpr float MIN_FOV = 1.0f;
    static constexpr float MAX_FOV = 179.0f;
    static constexpr float MIN_PITCH = -89.0f;
    static constexpr float MAX_PITCH = 89.0f;
    static constexpr float DEFAULT_NEAR_PLANE = 0.1f;
    static constexpr float DEFAULT_FAR_PLANE = 100.0f;

    Camera();

    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    void move(Camera_Movement direction, float deltaTime);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const glm::vec3& getPosition() const { return position_; }
    const glm::vec3& getFront() const { return front_; }

    void addObserver(ICameraObserver* observer);
    void removeObserver(ICameraObserver* observer);

    void onMouseMove(float xoffset, float yoffset);
    void onWindowResize(float width, float height);
    void setPosition(const glm::vec3& pos);
    void setFov(float fov);

    void setMovementSpeed(float speed) { movementSpeed_ = speed; }
    void setMouseSensitivity(float sensitivity) { mouseSensitivity_ = sensitivity; }
private:
    void updateCameraVectors();
    void notify() const;

    glm::vec3 position_; 
    glm::vec3 front_;    
    glm::vec3 up_;       
    glm::vec3 right_;    
    glm::vec3 worldUp_;  

    float yaw_;   
    float pitch_; 

    float fov_;          
    float aspectRatio_;  
    float nearPlane_;    
    float farPlane_;    

    float movementSpeed_ = DEFAULT_MOVEMENT_SPEED;
    float mouseSensitivity_ = DEFAULT_MOUSE_SENSITIVITY;

    std::vector<ICameraObserver*> observers_;
};