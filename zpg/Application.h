#pragma once

#include "Scene.h"

struct GLFWwindow;   
class ShaderProgram;    
class Model;

class Application {
public:
    Application();
    ~Application();

    void initialization();   
    void createShaders();    
    void createModels();     
    void run();              

private:
    float speed_ = 1.f;
    float x_ = 0.f;
    float y_ = 0.f;
    float z_ = 1.f;

    GLFWwindow* window_ = nullptr;
    ShaderProgram* progShader_ = nullptr;
    ShaderProgram* progShaderColor_ = nullptr;
    Model* modelTri_ = nullptr;
    Model* modelColor_ = nullptr;
    Model* modelSquare_ = nullptr;

    Scene scene_;

    bool ready_ = false;

    void onKey(int key, int scancode, int action, int mods);
};
