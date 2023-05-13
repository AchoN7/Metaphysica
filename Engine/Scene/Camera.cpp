#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera(vec3 position) {
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

void Camera::onViewportResized(int vpWidth, int vpHeight) {
    m_vpWidth = static_cast<float>(vpWidth);
    m_vpHeight = static_cast<float>(vpHeight);
    m_aspectRatio = m_vpWidth / m_vpHeight;
}

void Camera::move(Direction direction, float deltaTime) {
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

void Camera::mouseLook(float xoffset, float yoffset, bool constrainPitch) {
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

void Camera::scrollZoom(float yoffset) {
    if (m_zoom >= 1.0f && m_zoom <= 45.0f)
        m_zoom -= yoffset;
    if (m_zoom <= 1.0f)
        m_zoom = 1.0f;
    if (m_zoom >= 45.0f)
        m_zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    vec3 front;
    front.x = cos(radians(m_yaw)) * cos(radians(m_pitch));
    front.y = sin(radians(m_pitch));
    front.z = sin(radians(m_yaw)) * cos(radians(m_pitch));
    m_front = normalize(front);

    m_right = normalize(cross(m_front, m_worldup));
    m_up = normalize(cross(m_right, m_front));
}


mat4 Camera::getViewMatrix() const {
    return lookAt(m_position, m_position + m_front, m_up);
}

mat4 Camera::getProjectionMatrix() const {
    return perspective(radians(m_zoom), m_aspectRatio, 0.01f, 1000.0f);
}

vec3 Camera::getPosition() const { return m_position; }


void Camera::setPosition(vec3 newPos) { m_position = newPos; }
    