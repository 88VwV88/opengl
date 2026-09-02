#include "glengine/core/time.hpp"

namespace gln {

auto Time::now() -> double { return glfwGetTime(); }

auto Time::delta() const -> double { return delta_time; }

auto Time::tick() -> void {
  auto current_time = now();
  if (!initialized) {
    last_frame_time = current_time;
    last_fps_time = current_time;
    initialized = true;
  }

  delta_time = current_time - last_frame_time;
  last_frame_time = current_time;

  frames++;
  if (current_time - last_fps_time >= 1) {
    fps = frames;
    frametime = fps > 0 ? 1000.0 / fps : 0.0;
    last_fps_time = current_time;
    frames = 0;
  }
}

auto Time::get_frametime() const -> double { return frametime; }

auto Time::get_fps() const -> int { return fps; }

} // namespace gln
