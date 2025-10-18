#include "Controller.h"

Controller::Controller(Camera* camera) : camera_(camera) {}

void Controller::onMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            isLookingAround_ = true;
        }
        else if (action == GLFW_RELEASE) {
            isLookingAround_ = false;
        }
    }
}

void Controller::onMouseMove(double xpos, double ypos) {
    if (isLookingAround_) {
        float xoffset = xpos - lastX_;
        float yoffset = lastY_ - ypos; 

        camera_->onMouseMove(xoffset, yoffset);
    }

    lastX_ = xpos;
    lastY_ = ypos;
}

void Controller::update(GLFWwindow* window, float deltaTime) {
    if (!camera_) return;

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_->move(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_->move(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_->move(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_->move(Camera::RIGHT, deltaTime);
}

void Controller::onKey(int key, int scancode, int action, int mods) {
    
}