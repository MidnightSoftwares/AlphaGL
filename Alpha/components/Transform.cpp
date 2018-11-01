#include "Transform.h"

Transform::Transform() : Transform{glm::vec3{}} {}

Transform::Transform(const glm::vec3& position) : Transform{position, glm::vec3{}} {}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation) :
    Transform{position, rotation, glm::vec3{1.f}} {}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
    position_{position}, rotation_{rotation}, scale_{scale} {}

glm::vec3& Transform::position() {
    dirty_ = true;
    return position_;
}

const glm::vec3& Transform::position() const {
    return position_;
}

glm::vec3& Transform::rotation() {
    dirty_ = true;
    return rotation_;
}

const glm::vec3& Transform::rotations() const {
    return rotation_;
}

glm::vec3& Transform::scale() {
    dirty_ = true;
    return scale_;
}

const glm::vec3& Transform::scale() const {
    return scale_;
}

bool Transform::dirty() const {
    return dirty_;
}

void Transform::setDirty(bool dirty) {
    dirty_ = dirty;
}

const glm::mat4& Transform::modelMatrix() const {
    return modelMatrix_;
}

void Transform::setModelMatrix(const glm::mat4& modelMatrix) {
    modelMatrix_ = modelMatrix;
}
