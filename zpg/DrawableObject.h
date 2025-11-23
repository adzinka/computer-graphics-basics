#pragma once
#include <GL/glew.h>
#include "CompositeTransform.h"
#include "Texture.h"

class Model;
class ShaderProgram;

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* program,
        GLenum mode, GLsizei count, GLint first = 0)
        : model_(model), program_(program),
        mode_(mode), count_(count), first_(first),
        transform_(std::make_unique<CompositeTransform>()) {
    }

    void draw() const;

    void setTransform(std::unique_ptr<TransformComponent> transform);
    TransformComponent* getTransform() const { return transform_.get(); }

    ShaderProgram* getProgram() const { return program_; }
    void setColor(const glm::vec3& color) { color_ = color; }

    void setTexture(Texture* texture) { texture_ = texture; }
    Texture* getTexture() const { return texture_; }

    void setID(int id) { id_ = id; }
    int getID() const { return id_; }

    void setUvScale(float scale) { uvScale_ = scale; }

    void setDeletable(bool deletable) { isDeletable_ = deletable; }
    bool isDeletable() const { return isDeletable_; }

private:
    Model* model_ = nullptr;
    ShaderProgram* program_ = nullptr;
    GLenum   mode_ = GL_TRIANGLES;
    GLsizei  count_ = 0;
    GLint    first_;
    std::unique_ptr<TransformComponent> transform_ = nullptr;

    glm::vec3 color_ = glm::vec3(1.0f);
    Texture* texture_ = nullptr;
    float uvScale_ = 1.0f;

    int id_ = 0;
    bool isDeletable_ = true;
};
