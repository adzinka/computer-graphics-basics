#pragma once

#include <vector>
#include <memory>
#include "Scene.h"

struct GLFWwindow;   

class Application {
public:
    Application();
    ~Application();

    void initialization();      
    void createAndSetupScenes();
    void run();        
    void switchScene(int index);

private:
    GLFWwindow* window_ = nullptr;

    std::vector<std::unique_ptr<Scene>> scenes_;
    Scene* currentScene_ = nullptr;

    bool ready_ = false;

    void onKey(int key, int scancode, int action, int mods);
};
