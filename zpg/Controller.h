// Controller.h
#pragma once
#include "Camera.h"
#include <GLFW/glfw3.h> // Potřebujeme pro definice kláves

class Controller {
public:
    // Controller musí vědět, kterou kameru ovládá
    Controller(Camera* camera);

    // Metody, které bude volat Application z GLFW callbacks
    void onKey(int key, int scancode, int action, int mods);
    void onMouseButton(int button, int action, int mods);
    void onMouseMove(double xpos, double ypos);

    // Metoda, která se bude volat každý snímek pro plynulý pohyb
    void update(GLFWwindow* window, float deltaTime);

private:
    Camera* camera_; // Ukazatel na kameru, kterou ovládáme (nevlastníme ji)

    // Stav pro ovládání myší
    bool isLookingAround_ = false;
    double lastX_ = 0.0;
    double lastY_ = 0.0;
};