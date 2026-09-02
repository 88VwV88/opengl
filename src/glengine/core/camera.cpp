#include "glengine/core/camera.hpp"
#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace gln {

Camera::Camera(CameraInit const &init)
    : position{init.position}, world_up{init.world_up} {
  front = glm::normalize(init.front);
  yaw = glm::degrees(std::atan2(front.z, front.x));
  pitch = glm::degrees(std::asin(front.y));
  update_camera_vectors();
}

auto Camera::get_position() const -> glm::vec3 { return position; }

auto Camera::get_fov() const -> float { return zoom; }

auto Camera::get_view_matrix() const -> glm::mat4 {
  return glm::lookAt(position, position + front, up);
}

auto Camera::update_camera_vectors() -> void {
  front = glm::normalize(glm::vec3{
      std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
      std::sin(glm::radians(pitch)),
      std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
  });
  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}

auto Camera::move(float xpos, float ypos, bool constrain_pitch) -> void {
  if (is_first_mouse_event) {
    is_first_mouse_event = false;
    last_xpos = xpos;
    last_ypos = ypos;
  }

  float xoffset = sensitivity * (xpos - last_xpos);
  yaw += xoffset;

  float yoffset = sensitivity * (last_ypos - ypos);
  pitch += yoffset;

  last_xpos = xpos;
  last_ypos = ypos;

  if (constrain_pitch) {
    pitch = std::clamp(pitch, -89.0f, 89.0f);
  }
  update_camera_vectors();
}

auto Camera::strafe(double _delta_time, StrafeDirection const &direction)
    -> void {
  auto delta_time = static_cast<float>(_delta_time);
  float velocity = speed * delta_time;

  switch (direction) {
  case StrafeDirection::Left:
    position -= velocity * right;
    break;
  case StrafeDirection::Right:
    position += velocity * right;
    break;
  case StrafeDirection::Forward:
    position += velocity * front;
    break;
  case StrafeDirection::Backward:
    position -= velocity * front;
  }
}

auto Camera::scroll(float yoffset) -> void {
  zoom = std::clamp(zoom - yoffset, 1.0f, 45.0f);
}

} // namespace gln
