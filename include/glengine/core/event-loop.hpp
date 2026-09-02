#ifndef GLENGINE_EVENT_LOOP_HPP
#define GLENGINE_EVENT_LOOP_HPP

#include "glengine/core/time.hpp"
#include "glengine/utils.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>

namespace gln {

struct Window;

struct EventLoop {
  EventLoop() = default;
  GLN_DELETE_COPY_CTORS(EventLoop);
  EventLoop(EventLoop &&) = delete;
  auto operator=(EventLoop &&) -> EventLoop & = delete;

  auto bind(Window &window) -> void;
  auto poll_events() const -> void;
  auto tick() -> void;

  [[nodiscard]] auto get_time() const -> Time const &;

  static auto window_size_callback(GLFWwindow *glfw_window, std::int32_t width,
                                   std::int32_t height) -> void;
  static auto cursor_pos_callback(GLFWwindow *glfw_window, double xpos,
                                  double ypos) -> void;
  static auto scroll_callback(GLFWwindow *glfw_window, double, double yoffset)
      -> void;

private:
  auto process_keyboard_input() -> void;
  [[nodiscard]] auto get_window() -> Window &;

  Window *window{nullptr};
  GLFWwindow *native_window{nullptr};
  Time time;
  bool wireframe_enabled{false};
};

} // namespace gln

#endif
