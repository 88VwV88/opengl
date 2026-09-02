#ifndef GLENGINE_WINDOW_HPP
#define GLENGINE_WINDOW_HPP

#include "glengine/common.hpp"
#include "glengine/core/camera.hpp"
#include "glengine/core/time.hpp"
#include "glengine/utils.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>

namespace gln {

enum class OpenGLContextProfile : std::int32_t {
  Core = GLFW_OPENGL_CORE_PROFILE,
  Compat = GLFW_OPENGL_COMPAT_PROFILE,
};

struct ContextInit {
  std::int32_t major;
  std::int32_t minor;
  OpenGLContextProfile profile;
};

struct WindowInit {
  Dimensions screen;
  std::string const &title;
};

struct Context {
  Context(ContextInit const &init);
  GLN_DELETE_COPY_CTORS(Context);
  GLN_DEFINE_NOEXCEPT_MOVE_CTORS(Context);
  ~Context() noexcept;

  [[nodiscard]] auto init() const -> ExitCode; //!< Context initializer

private:
  ContextInit info;
};

struct Window {
  Window(ContextInit const &ctx_init);
  GLN_DELETE_COPY_CTORS(Window);
  GLN_DEFINE_NOEXCEPT_MOVE_CTORS(Window);
  ~Window() noexcept;

  [[nodiscard]] auto init(WindowInit const &init)
      -> ExitCode; //!< Window Initializer
  [[nodiscard]] auto should_close() const -> bool;
  [[nodiscard]] auto native_handle() const -> GLFWwindow *;

  auto swap_buffers() const -> void;
  static auto clear_color(SRGB const &color) -> void;
  auto update_title(Time const &time) -> void;
  auto resize(Dimensions dimensions) -> void;

  [[nodiscard]] auto get_camera() -> Camera &;
  [[nodiscard]] auto get_camera() const -> Camera const &;
  [[nodiscard]] auto get_view_matrix() const -> glm::mat4;
  [[nodiscard]] auto get_projection_matrix() const -> glm::mat4;

private:
  std::string title;
  GLFWwindow *window{nullptr};
  Dimensions screen{};
  Camera camera;

  Context context;
};

} // namespace gln

#endif
