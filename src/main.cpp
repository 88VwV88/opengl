#include "glengine/common.hpp"
#include "glengine/core/event-loop.hpp"
#include "glengine/core/light.hpp"
#include "glengine/core/program.hpp"
#include "glengine/model/scene.hpp"
#include "glengine/platform/window.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <string>

auto run_app() -> gln::ExitCode;
auto check_gl_error(std::uint64_t iter, GLenum &error) -> void;

int main() { return static_cast<std::int32_t>(run_app()); }

auto run_app() -> gln::ExitCode {
  GLenum error = GL_NO_ERROR;
  gln::Window window{{
      .major = 4,
      .minor = 6,
      .profile = gln::OpenGLContextProfile::Core,
  }};

  auto status = window.init({
      .screen = {.width = 800, .height = 600},
      .title = "GLFW Window",
  });
  if (gln::ExitCode::Success != status) {
    return status;
  }

  gln::EventLoop event_loop;
  event_loop.bind(window);

  gln::Program program;
  status =
      program.compile_shader_files({.vertex = "resources/shaders/mesh.vert",
                                    .fragment = "resources/shaders/mesh.frag"});
  if (gln::ExitCode::Success != status)
    return status;

  glEnable(GL_DEPTH_TEST);

  auto model_adamHead =
      gln::Scene::from_gltf2("resources/models/adamHead/", "adamHead.gltf");
  auto model_lieutenantHead = gln::Scene::from_gltf2(
      "resources/models/lieutenantHead/", "lieutenantHead.gltf");

  glm::mat4 model1{1.0f};
  model1 = glm::scale(model1, glm::vec3{0.5f});
  model1 =
      glm::rotate(model1, glm::radians(180.0f), glm::vec3{0.0f, 1.0f, 0.0f});
  model1 = glm::translate(model1, glm::vec3{1.0f, 0.0f, -0.5f});

  glm::mat4 model2{1.0f};
  model2 = glm::scale(model2, glm::vec3{0.5f});
  model2 =
      glm::rotate(model2, glm::radians(-105.0f), glm::vec3{0.0f, 1.0f, 0.0f});
  model2 = glm::translate(model2, glm::vec3{0.0f, 0.0f, -0.5f});

  gln::Light light{{
      .vertex = "resources/shaders/light.vert",
      .fragment = "resources/shaders/light.frag",
  }};

  while (!window.should_close()) {
    event_loop.poll_events();
    event_loop.tick();
    window.update_title(event_loop.get_time());

    gln::Window::clear_color({
        .red = 0.0f,
        .green = 0.0f,
        .blue = 0.0f,
        .alpha = 0.0f,
    });

    auto view = window.get_view_matrix();
    auto projection = window.get_projection_matrix();

    program.use();
    program.uniform("uCameraPosition", window.get_camera().get_position());
    program.uniform("uLight.position", window.get_camera().get_position());
    program.uniform("uLight.constant", 1.0f);
    program.uniform("uLight.linear", 0.09f);
    program.uniform("uLight.quadratic", 0.032f);
    program.uniform("uView", view);
    program.uniform("uProjection", projection);

    program.uniform("uModel", model1);
    model_adamHead.draw(program);

    program.uniform("uModel", model2);
    model_lieutenantHead.draw(program);

    light.draw({
        .model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
        .view = view,
        .projection = projection,
    });

    window.swap_buffers();
  }

  return gln::ExitCode::Success;
}

auto check_gl_error(std::uint64_t iter, GLenum &error) -> void {
  error = glGetError();
  if (GL_NO_ERROR != error) {
    throw std::runtime_error(
        std::format("[{}] [GL_ERROR]: {}", iter,
                    reinterpret_cast<const char *>(glewGetErrorString(error))));
  }
}
