#pragma once

#include "Scene.h"

struct GLFWwindow;   

class Application {
public:
    Application();
    ~Application();

    void initialization();      
    void setupScene();
    void run();              

private:
    float speed_ = 1.f;
    float x_ = 0.f;
    float y_ = 0.f;
    float z_ = 1.f;

    GLFWwindow* window_ = nullptr;

    Scene scene_;

    bool ready_ = false;

    void onKey(int key, int scancode, int action, int mods);
};
