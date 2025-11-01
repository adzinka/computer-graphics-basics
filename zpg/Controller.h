#pragma once
#include "Camera.h"
#include "Light.h"
#include <GLFW/glfw3.h> 

class Controller {
public:
    Controller(Camera* camera);

    void onKey(int key, int scancode, int action, int mods);
    void onMouseButton(int button, int action, int mods);
    void onMouseMove(double xpos, double ypos);

    void update(GLFWwindow* window, float deltaTime);

    void setFlashlight(Light* light) { flashlight_ = light; }
private:
    Camera* camera_; 

    bool isLookingAround_ = false;
    double lastX_ = 0.0;
    double lastY_ = 0.0;

    Light* flashlight_ = nullptr;
};