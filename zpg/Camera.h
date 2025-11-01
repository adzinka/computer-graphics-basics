#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <algorithm>

#include "ICameraObserver.h"

class Camera {
public:
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

    std::vector<ICameraObserver*> observers_;
};