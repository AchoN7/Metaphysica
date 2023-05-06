#pragma once

#include <glm/glm.hpp>

class Camera {
public:

    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

    void onViewportResized(int vpWidth, int vpHeight);
    void move(Direction direction, float deltaTime);
    void mouseLook(float xoffset, float yoffset, bool constrainPitch);
    void scrollZoom(float yoffset);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;

    void setPosition(glm::vec3 newPos);

private:

    float m_vpWidth;
    float m_vpHeight;
    float m_aspectRatio;

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_worldup;

    float m_yaw;
    float m_pitch;
    float m_zoom;

    float m_movementSpeed;
    float m_sensitivity;

    void updateCameraVectors();

};
