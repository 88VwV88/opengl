#include "glengine/core/event-loop.hpp"
#include "glengine/platform/window.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace gln {

namespace {

auto get_event_loop(GLFWwindow *glfw_window) -> EventLoop & {
  return *reinterpret_cast<EventLoop *>(glfwGetWindowUserPointer(glfw_window));
}

} // namespace

auto EventLoop::bind(Window &target_window) -> void {
  window = &target_window;
  native_window = target_window.native_handle();

  std::int32_t width{};
  std::int32_t height{};
  glfwGetWindowSize(native_window, &width, &height);
  target_window.resize({.width = width, .height = height});

  glfwSetWindowUserPointer(native_window, this);
  glfwSetScrollCallback(native_window, EventLoop::scroll_callback);
  glfwSetCursorPosCallback(native_window, EventLoop::cursor_pos_callback);
  glfwSetWindowSizeCallback(native_window, EventLoop::window_size_callback);
  glfwSetInputMode(native_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glViewport(0, 0, width, height);
}

auto EventLoop::poll_events() const -> void { glfwPollEvents(); }

auto EventLoop::tick() -> void {
  time.tick();
  process_keyboard_input();
}

auto EventLoop::get_time() const -> Time const & { return time; }

auto EventLoop::window_size_callback(GLFWwindow *glfw_window,
                                     std::int32_t width, std::int32_t height)
    -> void {
  auto &event_loop = get_event_loop(glfw_window);
  event_loop.get_window().resize({.width = width, .height = height});
  glViewport(0, 0, width, height);
}

auto EventLoop::cursor_pos_callback(GLFWwindow *glfw_window, double xpos,
                                    double ypos) -> void {
  get_event_loop(glfw_window)
      .get_window()
      .get_camera()
      .move(static_cast<float>(xpos), static_cast<float>(ypos));
}

auto EventLoop::scroll_callback(GLFWwindow *glfw_window, double,
                                double yoffset) -> void {
  get_event_loop(glfw_window)
      .get_window()
      .get_camera()
      .scroll(static_cast<float>(yoffset));
}

auto EventLoop::process_keyboard_input() -> void {
  if (native_window == nullptr) {
    return;
  }

  auto &camera = get_window().get_camera();
  auto delta_time = time.delta();
  if (GLFW_PRESS == glfwGetKey(native_window, GLFW_KEY_W)) {
    camera.strafe(delta_time, StrafeDirection::Forward);
  }
  if (GLFW_PRESS == glfwGetKey(native_window, GLFW_KEY_A)) {
    camera.strafe(delta_time, StrafeDirection::Left);
  }
  if (GLFW_PRESS == glfwGetKey(native_window, GLFW_KEY_S)) {
    camera.strafe(delta_time, StrafeDirection::Backward);
  }
  if (GLFW_PRESS == glfwGetKey(native_window, GLFW_KEY_D)) {
    camera.strafe(delta_time, StrafeDirection::Right);
  }

  if (GLFW_PRESS == glfwGetKey(native_window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(native_window, GLFW_TRUE);
  }

  auto should_enable_wireframe =
      GLFW_PRESS == glfwGetKey(native_window, GLFW_KEY_TAB);
  if (should_enable_wireframe != wireframe_enabled) {
    glPolygonMode(GL_FRONT_AND_BACK,
                  should_enable_wireframe ? GL_LINE : GL_FILL);
    wireframe_enabled = should_enable_wireframe;
  }
}

auto EventLoop::get_window() -> Window & { return *window; }

} // namespace gln
