#ifndef GLENGINE_TIME_HPP
#define GLENGINE_TIME_HPP

#include <GLFW/glfw3.h>

namespace gln {

struct Time {
  static auto now() -> double;
  [[nodiscard]] auto delta() const -> double;
  auto tick() -> void;
  [[nodiscard]] auto get_frametime() const -> double;
  [[nodiscard]] auto get_fps() const -> int;

private:
  int frames{0};
  int fps{0};
  double delta_time{0.0};
  double frametime{0.0};
  double last_frame_time{0.0};
  double last_fps_time{0.0};
  bool initialized{false};
};

} // namespace gln

#endif
