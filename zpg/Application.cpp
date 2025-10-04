//Include GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Include GLM
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "Application.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "DrawableObject.h"
#include "sphere.h"

static float points[] = {
    -0.85f, -0.75f, 0.0f,
    -0.65f, -0.95f, 0.0f,
    -0.95f, -0.95f, 0.0f
};

static float points2[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
};

static float square[] = {
     0.55f, 0.90f, 0.0f,    1.f, 0.f, 0.f,  
     0.55f, 0.50f, 0.0f,    0.f, 1.f, 0.f,  
     0.95f, 0.50f, 0.0f,    0.f, 0.f, 1.f,  

     0.55f, 0.90f, 0.0f,    1.f, 0.f, 0.f, 
     0.95f, 0.50f, 0.0f,    0.f, 0.f, 1.f,  
     0.95f, 0.90f, 0.0f,    1.f, 1.f, 0.f   
};

static const char* vertex_shader = R"(#version 330 core
layout(location=0) in vec3 vp;
void main () {
     gl_Position = vec4(vp, 1.0);
})";

static const char* fragment_shader = R"(#version 330 core
out vec4 fragColor;
void main () {
     fragColor = vec4 (0.5, 1.0, 0.5, 1.0);
})";

static const char* vs_color = R"(#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;
out vec3 vColor;
void main(){
    vec3 p = aPos;
    vColor = aColor;
    gl_Position = vec4(p, 1.0);
})";

static const char* fs_color = R"(#version 330 core
in vec3 vColor;

uniform vec4 shapeColor;

out vec4 fragColor;
void main(){
    fragColor = shapeColor;
})";

static const char* vs_color_matrix = R"(#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;

uniform mat4 modelMatrix; 

out vec3 vColor;
void main(){
    vColor = aColor;
    gl_Position = modelMatrix * vec4(aPos, 1.0);
})";

//static glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);
//static glm::mat4 View = glm::lookAt(
//    glm::vec3(10, 10, 10),
//    glm::vec3(0, 0, 0),
//    glm::vec3(0, 1, 0)
//);
//static glm::mat4 Model = glm::mat4(1.0f);


static void error_callback(int error, const char* description) { fputs(description, stderr); }

static void window_focus_callback(GLFWwindow* window, int focused) { printf("window_focus_callback \n"); }
static void window_iconify_callback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

static void window_size_callback(GLFWwindow* window, int width, int height) {
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}

static void cursor_callback(GLFWwindow* window, double x, double y) { printf("cursor_callback \n"); }
static void button_callback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
        printf("button_callback [%d,%d,%d]\n", button, action, mode);
    }
}


Application::Application() {}

Application::~Application() {

    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

void Application::initialization()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window_, this);

    // Callbacks
    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int sc, int act, int mods){
        if (auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w))) {
            app->onKey(key, sc, act, mods);
        }
    });
    glfwSetWindowFocusCallback(window_, window_focus_callback);
    glfwSetWindowIconifyCallback(window_, window_iconify_callback);
    glfwSetWindowSizeCallback(window_, window_size_callback);
    glfwSetCursorPosCallback(window_, cursor_callback);
    glfwSetMouseButtonCallback(window_, button_callback);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);

    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);
}

void Application::setupScene() {
 
    const int strideInBytes = 6 * sizeof(float);

    auto scene1 = std::make_unique<Scene>();
    { 
        ShaderProgram* progColor = scene1->makeProgram(vs_color_matrix, fs_color);

        Model* modelColor = scene1->makeModel(points2, sizeof(points2), strideInBytes);
        modelColor->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        modelColor->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float));
  
        DrawableObject* rectObj = scene1->addDrawable(modelColor, progColor, GL_TRIANGLES, 6);
        rectObj->getTransform().setPosition(glm::vec3(0.7f, 0.0f, 0.0f));
        rectObj->getTransform().setScale(glm::vec3(0.5f));
        
        Model* modelSphere = scene1->makeModel(sphere, sizeof(sphere), strideInBytes);
        modelSphere->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, strideInBytes, 0);
        modelSphere->enableAttrib(1, 3, GL_FLOAT, GL_FALSE, strideInBytes, 3 * sizeof(float));

        DrawableObject* sphereObj = scene1->addDrawable(modelSphere, progColor, GL_TRIANGLES, modelSphere->getVertexCount());
        sphereObj->getTransform().setPosition(glm::vec3(-0.5f, -0.4f, 0.0f));
        sphereObj->getTransform().setScale(glm::vec3(0.5f));
        
    }
    scenes_.push_back(std::move(scene1));

    auto scene2 = std::make_unique<Scene>();
    {
        ShaderProgram* progSimple = scene2->makeProgram(vertex_shader, fragment_shader);
        Model* modelTri = scene2->makeModel(points, sizeof(points), strideInBytes);
        modelTri->enableAttrib(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        scene2->addDrawable(modelTri, progSimple, GL_TRIANGLES, 3);
    }
    scenes_.push_back(std::move(scene2));

    switchScene(0); 
    ready_ = true; 

}

void Application::run()
{
    if (!window_) return;
    if (!ready_) return;

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window_)) {
        // clear color and depth buffer
        

        float time = glfwGetTime();
        currentScene_->update(time);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //scene_.drawAll();
        currentScene_->drawAll();

        glfwPollEvents();
        glfwSwapBuffers(window_);
    }
}

void Application::switchScene(int index) {
    if (index >= 0 && index < scenes_.size()) {
        currentScene_ = scenes_[index].get();
        printf("Switched to scene %d\n", index);
    }
    else {
        printf("Error: scene index %d is out of bounds.\n", index);
    }
}

void Application::onKey(int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window_, GL_TRUE);
            break;
        case GLFW_KEY_1:
            switchScene(0);
            break;
        case GLFW_KEY_2:
            switchScene(1);
            break;
        case GLFW_KEY_RIGHT: x_ = 1.f; break;
        case GLFW_KEY_LEFT:  x_ = -1.f; break;
        case GLFW_KEY_X: x_ = 1.f; y_ = 0.f; z_ = 0.f; break;
        case GLFW_KEY_Y: x_ = 0.f; y_ = 1.f; z_ = 0.f; break;
        case GLFW_KEY_Z: x_ = 0.f; y_ = 0.f; z_ = 1.f; break;
        default: break;
    }
    printf("onKey [%d,%d]  state: x=%.1f y=%.1f z=%.1f\n", key, mods, x_, y_, z_);
}
