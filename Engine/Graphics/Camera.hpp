#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera {
public:

    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(vec3 position = vec3(0.0f, 0.0f, 1.0f)) {
        m_position = position;
        m_worldup = vec3(0.0f, 1.0f, 0.0f);
        m_front = vec3(0.0f, 0.0f, -1.0f);
        m_up = m_worldup;
        m_right = glm::vec3();
        
        m_yaw = -90.0f,
        m_pitch = 0.0f;

        m_movementSpeed = 20.5f;
        m_sensitivity = 0.2f;
        m_zoom = 45.0f;

        updateCameraVectors();
    }

    void setPosition(glm::vec3 newPos) { m_position = newPos; }
    void setLimits()

    mat4 getViewMatrix() {
        return lookAt(m_position, m_position + m_front, m_up);
    }

    mat4 getProjectionMatrix() {
        return perspective(radians(m_zoom), m_aspectRatio, 0.01f, 20000.0f);
    }

    glm::vec3 getPosition() const { return m_position; }

    void onViewportResized(int vpWidth, int vpHeight) {
        m_vpWidth = static_cast<float>(vpWidth);
        m_vpHeight = static_cast<float>(vpHeight);
        m_aspectRatio = m_vpWidth / m_vpHeight;
    }

    void move(Direction direction, float deltaTime) {
        float velocity = m_movementSpeed * deltaTime;
        if (direction == Direction::FORWARD)
            m_position += m_front * velocity;
        if (direction == Direction::BACKWARD)
            m_position -= m_front * velocity;
        if (direction == Direction::LEFT)
            m_position -= m_right * velocity;
        if (direction == Direction::RIGHT)
            m_position += m_right * velocity;
    }

    void mouseLook(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= m_sensitivity;
        yoffset *= m_sensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (constrainPitch) {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void scrollZoom(float yoffset) {
        if (m_zoom >= 1.0f && m_zoom <= 45.0f)
            m_zoom -= yoffset;
        if (m_zoom <= 1.0f)
            m_zoom = 1.0f;
        if (m_zoom >= 45.0f)
            m_zoom = 45.0f;
    }

private:

    float m_vpWidth;
    float m_vpHeight;
    float m_aspectRatio;

    vec3 m_position;
    vec3 m_front;
    vec3 m_right;
    vec3 m_up;
    vec3 m_worldup;

    float m_yaw;
    float m_pitch;
    float m_zoom;

    float m_movementSpeed;
    float m_sensitivity;

    void updateCameraVectors() {
        vec3 front;
        front.x = cos(radians(m_yaw)) * cos(radians(m_pitch));
        front.y = sin(radians(m_pitch));
        front.z = sin(radians(m_yaw)) * cos(radians(m_pitch));
        m_front = normalize(front);

        m_right = normalize(cross(m_front, m_worldup));
        m_up = normalize(cross(m_right, m_front));
    }

};
