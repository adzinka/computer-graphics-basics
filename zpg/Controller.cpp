#include <GL/glew.h>
#include "Controller.h"
#include "Application.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Translate.h"
#include "Scale.h"
#include <glm/gtc/matrix_transform.hpp> // unProject
#include <iostream>

Controller::Controller(Camera* camera, Application* app) 
    : camera_(camera), app_(app) {}

void Controller::onMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            isLookingAround_ = true;
        }
        else if (action == GLFW_RELEASE) {
            isLookingAround_ = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (!app_ || !camera_) return;

        Scene* scene = app_->getCurrentScene();
        GLFWwindow* window = app_->getWindow();
        if (!scene || !window) return;

        if (!scene->allowsObjectPicking() && !scene->allowsObjectPlanting()) {
            return;
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        GLint x = (GLint)lastX_;
        GLint y = (GLint)lastY_;
        GLint newy = height - y;

        glReadBuffer(GL_BACK); 
        GLfloat depth;
        GLuint index;
        glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        printf("Clicked on pixel %d, %d (flipped %d), depth %f, stencil index %u\n",
            x, y, newy, depth, index);

        glm::vec3 screenPos = glm::vec3(x, newy, depth);
        glm::mat4 view = camera_->getViewMatrix();
        glm::mat4 projection = camera_->getProjectionMatrix();
        glm::vec4 viewport = glm::vec4(0, 0, width, height);

        glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);
        printf("unProject world position: [%f, %f, %f]\n", worldPos.x, worldPos.y, worldPos.z);

        DrawableObject* clickedObject = scene->getObjectByID(index);

        if (mods == GLFW_MOD_SHIFT && scene->allowsObjectPlanting()) {
            bool canPlant = (index == 0 || (clickedObject && !clickedObject->isDeletable()));

            if (canPlant && depth < 1.0) { 
                printf("Planting tree at: %f, %f, %f\n", worldPos.x, worldPos.y, worldPos.z);

                ResourceManager* manager = app_->getResourceManager();
                Model* treeModel = manager->getModel("tree");
                ShaderProgram* progPhong = manager->getShader("phong");

                if (treeModel && progPhong) {
                    scene->addObject(treeModel, progPhong, worldPos);
                }
            }
        }

        else if (mods == GLFW_MOD_CONTROL && scene->allowsObjectPlanting()) {

            bool canPlant = (index == 0 || (clickedObject && !clickedObject->isDeletable()));

            if (canPlant && depth < 1.0) {
                printf("Adding Shrek at: %f, %f, %f\n", worldPos.x, worldPos.y, worldPos.z);

                ResourceManager* manager = app_->getResourceManager();
                Model* shrekModel = manager->getModel("shrek");
                ShaderProgram* progTexture = manager->getShader("texture_light");
                Texture* shrekTexture = manager->getTexture("shrek_texture");

                if (shrekModel && progTexture && shrekTexture) {
                    scene->addCharacter(shrekModel, progTexture, shrekTexture, worldPos);
                }
            }
        }
        else if (scene->allowsObjectPicking()) {

            if (clickedObject && clickedObject->isDeletable()) {
                printf("Deleting object with ID: %u\n", index);
                scene->deleteObjectByID(index);
            }
            else if (clickedObject) {
                printf("Clicked on non-deletable object (ID %u).\n", index);
            }
        }
    }
}

void Controller::onMouseMove(double xpos, double ypos) {
    if (isLookingAround_) {
        float xoffset = xpos - lastX_;
        float yoffset = lastY_ - ypos; 

        camera_->onMouseMove(xoffset, yoffset);
    }

    lastX_ = xpos;
    lastY_ = ypos;
}

void Controller::update(GLFWwindow* window, float deltaTime) {
    if (!camera_) return;

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_->move(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_->move(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_->move(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_->move(Camera::RIGHT, deltaTime);
}

void Controller::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        if (flashlight_) {
            flashlight_->setEnabled(!flashlight_->isEnabled());
        }
    }
}