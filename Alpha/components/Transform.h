#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Transform {
public:
    Transform();
    explicit Transform(const glm::vec3& position);
    Transform(const glm::vec3& position, const glm::vec3& rotation);
    Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

public:
    glm::vec3& position();
    const glm::vec3& position() const;
    glm::vec3& rotation();
    const glm::vec3& rotations() const;
    glm::vec3& scale();
    const glm::vec3& scale() const;
    bool dirty() const;
    void setDirty(bool dirty);
    const glm::mat4& modelMatrix() const;
    void setModelMatrix(const glm::mat4& modelMatrix);

private:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;
    mutable bool dirty_{true};
    glm::mat4 modelMatrix_{1.f};
};
