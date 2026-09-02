#include "glengine/platform/window.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <format>
#include <glm/gtc/matrix_transform.hpp>
#include <print>
#include <stdexcept>

namespace gln {

Window::Window(ContextInit const &ctx_init) : context{ctx_init} {}

Window::~Window() noexcept {
  if (window != nullptr) {
    glfwDestroyWindow(window);
  }
}

auto Window::init(WindowInit const &init) -> ExitCode {
  title = init.title;
  screen = init.screen;
  window = glfwCreateWindow(init.screen.width, init.screen.height,
                            init.title.c_str(), nullptr, nullptr);
  if (nullptr != window) {
    glfwMakeContextCurrent(window);
    return context.init();
  }

#ifndef NDEBUG
  throw std::runtime_error("Failed to initialize GLFW window");
#endif
  return ExitCode::FailedToInitalizeWindow;
}

auto Window::should_close() const -> bool {
  return GLFW_TRUE == glfwWindowShouldClose(window);
}

auto Window::native_handle() const -> GLFWwindow * { return window; }

auto Window::swap_buffers() const -> void { glfwSwapBuffers(window); }

auto Window::clear_color(SRGB const &color) -> void {
  glClearColor(color.red, color.green, color.blue, color.alpha);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Context::Context(ContextInit const &init) : info{init} {
  if (GLFW_FALSE != glfwInit()) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, init.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, init.minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   static_cast<std::int32_t>(init.profile));
  }
}

auto Window::update_title(Time const &time) -> void {
  glfwSetWindowTitle(window, std::format("{} | FPS: {} | Frametime: {:.2f}ms",
                                         title, time.get_fps(),
                                         time.get_frametime())
                                 .c_str());
}

auto Window::resize(Dimensions dimensions) -> void { screen = dimensions; }

auto Window::get_camera() -> Camera & { return camera; }

auto Window::get_camera() const -> Camera const & { return camera; }

auto Window::get_view_matrix() const -> glm::mat4 {
  return camera.get_view_matrix();
}

auto Window::get_projection_matrix() const -> glm::mat4 {
  auto aspect_ratio = 1.0f;
  if (screen.height != 0) {
    aspect_ratio = static_cast<float>(screen.width) /
                   static_cast<float>(screen.height);
  }

  return glm::perspective(glm::radians(camera.get_fov()), aspect_ratio, 0.1f,
                          100.0f);
}

Context::~Context() noexcept { glfwTerminate(); }

auto Context::init() const -> ExitCode {
  auto status = glewInit();

#ifndef NDEBUG
  if (GLEW_OK != status && GLEW_ERROR_NO_GLX_DISPLAY != status) {
    std::string error_string{
        reinterpret_cast<const char *>(glewGetErrorString(status))};
    throw std::runtime_error("Failed to initialize OpenGL context: " +
                             error_string);
  } else {
    std::println("Using OpenGL version: {}",
                 reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  }
#endif

  return (GLEW_OK == status || GLEW_ERROR_NO_GLX_DISPLAY == status)
             ? ExitCode::Success
             : ExitCode::FailedToInitializeOpenGL;
}

} // namespace gln
