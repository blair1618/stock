//
// Created by Matt Blair on 8/5/16.
//
#pragma once

#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace stock {

class ShaderProgram;

class Camera {

public:

    enum class Type : uint8_t {
        perspective,
        orthographic,
    };

    Camera();

    const glm::vec3& position();
    const glm::vec3& upVector();
    const glm::vec3& direction();

    void setPosition(const glm::vec3& position);
    void setPosition(float x, float y, float z);

    void setUpVector(const glm::vec3& up);
    void setUpVector(float x, float y, float z);

    void setDirection(const glm::vec3& direction);
    void setDirection(float x, float y, float z);

    void lookAt(const glm::vec3& target);
    void lookAt(float x, float y, float z);

    void translate(const glm::vec3& displacement);
    void translate(float x, float y, float z);

    void rotate(const glm::vec3& axis, float angle);
    void rotate(float axisX, float axisY, float axisZ, float angle);

    void orbit(const glm::vec3& target, const glm::vec3& axis, float angle);
    void orbit(float tX, float tY, float tZ, float aX, float aY, float aZ, float angle);

    void update();

    void apply(ShaderProgram& shader);

private:

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;
    glm::mat4 m_viewProjMatrix;
    glm::mat4 m_invViewProjMatrix;
    glm::mat3 m_normalMatrix;
    glm::mat3 m_invNormalMatrix;
    glm::vec3 m_position;
    glm::vec3 m_up = { 0.f, 1.f, 0.f };
    glm::vec3 m_direction { 0.f, 0.f, 1.f };
    float m_width = 1.f;
    float m_height = 1.f;
    float m_fov = 1.5708f;
    Type m_type = Type::perspective;
    bool m_dirty = true;

};

} // namespace stock
