#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <algorithm>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

struct Camera {
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float Yaw;
  float Pitch;

  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
  }

  glm::mat4 GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
  }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    switch (direction) {
    case FORWARD:
      Position += Front * velocity;
      break;
    case BACKWARD:
      Position -= Front * velocity;
      break;
    case LEFT:
      Position -= Right * velocity;
      break;
    case RIGHT:
      Position += Right * velocity;
      break;
    default:
      throw std::runtime_error("UNREACHABLE: Camera_Movement");
    }
  }

  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
      Pitch = std::clamp(Pitch, -89.0f, 89.0f);

    updateCameraVectors();
  }

  void ProcessMouseScroll(float yoffset) {
    Zoom = std::clamp(Zoom - yoffset, 1.0f, 45.0f);
  }

private:
  void updateCameraVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
    front.y = std::sin(glm::radians(Pitch));
    front.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

#endif