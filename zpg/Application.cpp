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
#include <string> 

#include "Application.h"
#include "TriangleScene.h"
#include "SpheresScene.h"
#include "SpheresScene2.h"
#include "ForestScene.h"
#include "SolarSystemScene.h"
#include "TestScene.h"
#include "GameScene.h"

#include "sphere.h"
#include "suzi_smooth.h"
#include "tree.h"
#include "bushes.h"
#include "gift.h"
#include "plain.h"
#include "plain_uv.h"
#include "sky_cube.h"

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

static float triangle_vertices[] = {
   0.0f, 0.6f, 0.0f,      1.0f, 0.0f, 0.0f,
   -0.52f, -0.3f, 0.0f,   0.0f, 1.0f, 0.0f,
   0.52f, -0.3f, 0.0f,    0.0f, 0.0f, 1.0f
};

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

    window_ = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window_, this);

    controller_ = std::make_unique<Controller>(&camera_, this);

    // Callbacks
    glfwSetKeyCallback(window_, [](GLFWwindow* w, int key, int sc, int act, int mods){
        if (auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w))) {
            app->onKey(key, sc, act, mods);

            app->controller_->onKey(key, sc, act, mods);
        }
    });
    glfwSetWindowFocusCallback(window_, window_focus_callback);
    glfwSetWindowIconifyCallback(window_, window_iconify_callback);
    glfwSetWindowSizeCallback(window_, window_size_callback);
    glfwSetCursorPosCallback(window_, cursor_callback);

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window_, [](GLFWwindow* w, double xpos, double ypos) {
        if (auto app = static_cast<Application*>(glfwGetWindowUserPointer(w))) {

            app->controller_->onMouseMove(xpos, ypos);
        }
    });
    glfwSetMouseButtonCallback(window_, [](GLFWwindow* w, int button, int action, int mods) {
        if (auto app = static_cast<Application*>(glfwGetWindowUserPointer(w))) {
            app->controller_->onMouseButton(button, action, mods);
        }
        });
    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* w, int width, int height) {
        if (auto app = static_cast<Application*>(glfwGetWindowUserPointer(w))) {
            app->onWindowResize(width, height);
        }
        });

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

    loadResources();
}

void Application::createAndSetupScenes() {
 
    auto scene1 = std::make_unique<TriangleScene>();
    scene1->setup(camera_, resourceManager_);
    scenes_.push_back(std::move(scene1));

    auto scene2 = std::make_unique<SpheresScene>();
    scene2->setup(camera_, resourceManager_);
    scenes_.push_back(std::move(scene2));

    auto scene3 = std::make_unique<ForestScene>();
    scene3->setup(camera_, resourceManager_);
    scenes_.push_back(std::move(scene3));

    auto scene4 = std::make_unique<SolarSystemScene>();
    scene4->setup(camera_, resourceManager_);
    scenes_.push_back(std::move(scene4));

    auto scene5 = std::make_unique<SpheresScene2>();
    scene5->setup(camera_, resourceManager_);
    scenes_.push_back(std::move(scene5));

    auto scene6 = std::make_unique<GameScene>();
    scene6->setup(camera_, resourceManager_);
    scenes_.push_back(std::move(scene6));

    switchScene(0); 
    ready_ = true; 
}

void Application::run()
{
    if (!window_ || !ready_) return;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    while (!glfwWindowShouldClose(window_)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime_ = currentFrame - lastFrame_;
        lastFrame_ = currentFrame;

        if (controller_) {
            controller_->update(window_, deltaTime_);
        }
        
        if (currentScene_) {
            currentScene_->update(currentFrame, camera_);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 
        glClearStencil(0);

        glStencilMask(0xFF);

        if (currentScene_) {
            currentScene_->drawAll(camera_);
        }

        glfwPollEvents();
        glfwSwapBuffers(window_);
    }
}

void Application::switchScene(int index) {
    if (index >= 0 && index < scenes_.size()) {
        currentScene_ = scenes_[index].get();
        printf("Switched to scene %d\n", index);

        if (controller_) {
            Light* flashlight = currentScene_->getFlashlight();
            controller_->setFlashlight(flashlight);

            if (flashlight) {
                printf("Flashlight linked to Controller.\n");
            }
            else {
                printf("No flashlight in this scene.\n");
            }
        }
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
        case GLFW_KEY_3:
            switchScene(2);
            break;
        case GLFW_KEY_4:
            switchScene(3);
            break;
        case GLFW_KEY_5:
            switchScene(4);
            break;
        case GLFW_KEY_6:
            switchScene(5); 
            break;
        case GLFW_KEY_F1:
            camera_.setFov(45.0f);
            printf("FOV 45\n");
            break;
        case GLFW_KEY_F2:
            camera_.setFov(90.0f);
            printf("FOV 90\n");
            break;
        case GLFW_KEY_F3:
            camera_.setFov(130.0f);
            printf("FOV 130\n");
            break;
        default: break;
    }
    printf("onKey [%d,%d]\n", key, mods);
}

void Application::onWindowResize(int width, int height) {
    glViewport(0, 0, width, height);
    camera_.onWindowResize(static_cast<float>(width), static_cast<float>(height));
}

void Application::loadResources() {

    size_t stride = 6 * sizeof(float);

    resourceManager_.createShader("constant", std::string("shaders/lighting.vert"), std::string("shaders/constant.frag"), &camera_);
    resourceManager_.createShader("lambert", std::string("shaders/lighting.vert"), std::string("shaders/lambert.frag"), &camera_);
    resourceManager_.createShader("phong", std::string("shaders/lighting.vert"), std::string("shaders/phong.frag"), &camera_);
    resourceManager_.createShader("blinn", std::string("shaders/lighting.vert"), std::string("shaders/blinn.frag"), &camera_);
    resourceManager_.createShader("bad_phong", std::string("shaders/lighting.vert"), std::string("shaders/bad_phong.frag"), &camera_);

    resourceManager_.createShader("texture", "shaders/texture.vert", "shaders/texture.frag", &camera_);
    resourceManager_.createShader("texture_light", "shaders/texture.vert", "shaders/texture_phong.frag", &camera_);

    resourceManager_.createShader("skybox", "shaders/skybox.vert", "shaders/skybox.frag", nullptr);

    resourceManager_.createModel("triangle", triangle_vertices, sizeof(triangle_vertices), stride, true);
    resourceManager_.createModel("sphere", sphere, sizeof(sphere), stride, true);
    resourceManager_.createModel("suzi", suzi_smooth, sizeof(suzi_smooth), stride, true);
    resourceManager_.createModel("tree", tree, sizeof(tree), stride, true);
    resourceManager_.createModel("bush", bushes, sizeof(bushes), stride, true);
    resourceManager_.createModel("gift", gift, sizeof(gift), stride, false);
    resourceManager_.createModel("plain", plain, sizeof(plain), stride, true);

    resourceManager_.createModel("skybox_cube", skycube, sizeof(skycube), 3 * sizeof(float), false);

    size_t stride_uv = 8 * sizeof(float);  

    resourceManager_.createModel("plain_uv", plain_uv, sizeof(plain_uv), stride_uv, true);

    resourceManager_.loadModel("cube", std::string("assets/cube.obj"));
    resourceManager_.loadModel("formula1", std::string("assets/formula1.obj"));
    resourceManager_.loadModel("house", std::string("assets/house.obj"));
    resourceManager_.loadModel("shrek", "assets/shrek.obj");
    resourceManager_.loadModel("fiona", "assets/fiona.obj");
    resourceManager_.loadModel("toiled", "assets/toiled.obj");
    resourceManager_.loadModel("teren", "assets/teren.obj");
    resourceManager_.loadModel("duck", "assets/12249_Bird_v1_L2.obj");
    resourceManager_.loadModel("planet", "assets/planet.obj");

    resourceManager_.loadTexture("grass", "assets/grass.png");
    resourceManager_.loadTexture("wood", "assets/wooden_fence.png");
    resourceManager_.loadTexture("shrek_texture", "assets/shrek.png");
    resourceManager_.loadTexture("fiona_texture", "assets/fiona.png");
    resourceManager_.loadTexture("toiled_texture", "assets/toiled.jpg");
    resourceManager_.loadTexture("duck_texture", "assets/12249_Bird_v1_diff.jpg");
    resourceManager_.loadTexture("sun_texture", "assets/2k_sun.jpg");
    resourceManager_.loadTexture("earth_texture", "assets/2k_earth_daymap.jpg");
    resourceManager_.loadTexture("moon_texture", "assets/2k_moon.jpg");
    resourceManager_.loadTexture("mars_texture", "assets/2k_mars.jpg");

    int maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    printf("Max texture units: %d\n", maxTextureUnits);

    std::vector<std::string> faces = {
        "assets/posx.jpg",
        "assets/negx.jpg",
        "assets/posy.jpg",
        "assets/negy.jpg",
        "assets/posz.jpg",
        "assets/negz.jpg"
    };
    resourceManager_.loadCubemap("mainSkybox", faces);

}