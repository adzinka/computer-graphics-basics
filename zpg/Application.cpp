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
#include "TriangleScene.h"
#include "SpheresScene.h"
#include "ForestScene.h"

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

    window_ = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
    if (!window_) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window_, this);

    controller_ = std::make_unique<Controller>(&camera_);

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
        // Získáme ukazatel na naši aplikaci
        if (auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w))) {
            // A zavoláme na ní naši metodu
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
}

void Application::createAndSetupScenes() {
 
    auto scene1 = std::make_unique<TriangleScene>();
    scene1->setup(camera_);
    scenes_.push_back(std::move(scene1));

    auto scene2 = std::make_unique<SpheresScene>();
    scene2->setup(camera_);
    scenes_.push_back(std::move(scene2));

    auto scene3 = std::make_unique<ForestScene>();
    scene3->setup(camera_);
    scenes_.push_back(std::move(scene3));

    switchScene(0); 
    ready_ = true; 
}

void Application::run()
{
    if (!window_ || !ready_) return;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window_)) {
        // --- KROK 1: Výpočet deltaTime pro plynulý pohyb ---
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime_ = currentFrame - lastFrame_;
        lastFrame_ = currentFrame;

        // --- KROK 2: Zpracování vstupu pro plynulý pohyb (WSAD) ---
        // Řekneme controlleru, aby zkontroloval stisknuté klávesy a pohnul kamerou
        if (controller_) {
            controller_->update(window_, deltaTime_);
        }
        
        // --- KROK 3: Aktualizace logiky scény (animace objektů) ---
        // Toto volání zůstává stejné
        if (currentScene_) {
            currentScene_->update(currentFrame);
        }

        // --- KROK 4: Vykreslení ---
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        if (currentScene_) {
            currentScene_->drawAll();
        }

        // --- KROK 5: Závěrečné operace ---
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
        case GLFW_KEY_3:
            switchScene(2);
            break;
        default: break;
    }
    printf("onKey [%d,%d]\n", key, mods);
}

void Application::onWindowResize(int width, int height) {
    // Nastaví novou velikost framebufferu pro OpenGL
    glViewport(0, 0, width, height);
    // Předá novou velikost naší kameře, aby si přepočítala poměr stran
    camera_.onWindowResize(static_cast<float>(width), static_cast<float>(height));
}