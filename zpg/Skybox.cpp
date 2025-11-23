#include "Skybox.h"

Skybox::Skybox(Model* model, ShaderProgram* shader, GLuint cubemapTexID)
    : model_(model), shader_(shader), cubemapTexID_(cubemapTexID) {
}

void Skybox::draw(const Camera& camera) const {
    if (!model_ || !shader_ || cubemapTexID_ == 0) {
        return;
    }

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    // Disable writing to the Stencil Buffer so the skybox doesn't have an ID
    glStencilMask(0x00);

    shader_->useProgram();

    shader_->setUniform("viewMatrix", camera.getViewMatrix());
    shader_->setUniform("projectionMatrix", camera.getProjectionMatrix());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexID_);
    shader_->setUniform("skybox", 0); 

    model_->draw(GL_TRIANGLES, 0, 36);

    shader_->unuseProgram();

    // Re-enable writing for the rest of the objects
    glStencilMask(0xFF);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}