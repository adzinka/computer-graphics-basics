// Controller.cpp
#include "Controller.h"

Controller::Controller(Camera* camera) : camera_(camera) {}

void Controller::onMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            isLookingAround_ = true;
            // Můžeme zde uložit počáteční pozici myši, pokud bychom nechtěli "skok"
        }
        else if (action == GLFW_RELEASE) {
            isLookingAround_ = false;
        }
    }
}

void Controller::onMouseMove(double xpos, double ypos) {
    // Otáčíme kamerou, POUZE pokud držíme pravé tlačítko
    if (isLookingAround_) {
        // Jednoduchá implementace bez ukládání pozice při stisku
        // Bude docházet ke skoku při prvním pohybu, což je pro začátek v pořádku
        float xoffset = xpos - lastX_;
        float yoffset = lastY_ - ypos; // Obráceně

        camera_->onMouseMove(xoffset, yoffset);
    }
    // Vždy aktualizujeme poslední pozici, abychom při příštím pohybu měli správný rozdíl
    lastX_ = xpos;
    lastY_ = ypos;
}

void Controller::update(GLFWwindow* window, float deltaTime) {
    if (!camera_) return;

    float cameraSpeed = 2.5f * deltaTime; // deltaTime zajišťuje plynulý pohyb nezávislý na FPS

    // Zpracování kláves WSAD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_->move(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_->move(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_->move(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_->move(Camera::RIGHT, deltaTime);
}

// Metoda onKey zde může zůstat prázdná, protože pohyb řešíme v update.
// Můžete ji použít pro jednorázové akce (např. skok).
void Controller::onKey(int key, int scancode, int action, int mods) {
    // Například: if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { ... }
}