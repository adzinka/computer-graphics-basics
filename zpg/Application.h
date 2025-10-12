#pragma once

#include <vector>
#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "Controller.h"

struct GLFWwindow;   

class Application {
public:
    Application();
    ~Application();

    void initialization();      
    void createAndSetupScenes();
    void run();        
    void switchScene(int index);

    Camera& getCamera() { return camera_; }
    void onWindowResize(int width, int height);

private:
    GLFWwindow* window_ = nullptr;
    Camera camera_;
    std::unique_ptr<Controller> controller_;

    std::vector<std::unique_ptr<Scene>> scenes_;
    Scene* currentScene_ = nullptr;

    bool ready_ = false;

    void onKey(int key, int scancode, int action, int mods);

    float deltaTime_ = 0.0f;
    float lastFrame_ = 0.0f;
};
