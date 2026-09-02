#ifndef GLENGINE_CAMERA_HPP
#define GLENGINE_CAMERA_HPP

#include <cstdint>
#include <glm/glm.hpp>

namespace gln {

struct CameraInit {
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 world_up;
};

enum class StrafeDirection : std::uint8_t {
  Left,
  Right,
  Forward,
  Backward,
};

static constexpr CameraInit DEFAULT_CAMERA_INIT = {
    .position = {0.0f, 0.0f, 3.0f},
    .front = {0.0f, 0.0f, -1.0f},
    .world_up = {0.0f, 1.0f, 0.0f},
};

struct Camera {
  Camera(CameraInit const &init = DEFAULT_CAMERA_INIT);

  auto move(float xpos, float ypos, bool constrain_pitch = true) -> void;
  auto scroll(float yoffset) -> void;
  auto strafe(double delta_time, StrafeDirection const &direction) -> void;
  auto update_camera_vectors() -> void;

  [[nodiscard]] auto get_fov() const -> float;
  [[nodiscard]] auto get_position() const -> glm::vec3;
  [[nodiscard]] auto get_view_matrix() const -> glm::mat4;

private:
  glm::vec3 position;
  glm::vec3 world_up;
  glm::vec3 up;
  glm::vec3 front;
  glm::vec3 right;

  float speed = {2.5f};
  float sensitivity = {0.1f};

  float zoom = {45.0f};
  float yaw = {-90.0f};
  float pitch = {0.0f};

  float last_xpos = {0.0f};
  float last_ypos = {0.0f};
  bool is_first_mouse_event = {true};
};

} // namespace gln

#endif
