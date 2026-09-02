#ifndef GLENGINE_LIGHT_HPP
#define GLENGINE_LIGHT_HPP

#include "glengine/common.hpp"
#include "glengine/core/program.hpp"
#include "glengine/geometry/buffers.hpp"
#include "glengine/geometry/vertex-array.hpp"
namespace gln {

struct LightDrawMatrices {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
};

static constexpr std::array<Vertex, 5> LIGHT_VERTICES = {{
    {.position = glm::vec3{-0.5f, 0.0f, -0.5f}},
    {.position = glm::vec3{0.5f, 0.0, -0.5f}},
    {.position = glm::vec3{0.5f, 0.0f, 0.5f}},
    {.position = glm::vec3{-0.5f, 0.0f, 0.5f}},
    {.position = glm::vec3{0.0f, 1.0f, 0.0f}},
}};
static constexpr std::array<std::uint32_t, 18> LIGHT_INDICES = {{0, 2, 1, //
                                                                 0, 3, 2, //
                                                                 0, 1, 4, //
                                                                 1, 2, 4, //
                                                                 2, 3, 4, //
                                                                 3, 0, 4}};
static constexpr auto LIGHT_COLOR = glm::vec3{1.0f, 1.0f, 1.0f};

struct Light {
  Light(ShaderFiles const &files);
  auto draw(LightDrawMatrices const &params) const -> void;
  [[nodiscard]] auto status() const -> ExitCode;

private:
  Program program;
  VertexArray vertex_array;
  VertexBuffer vertex_buffer;
  ElementBuffer element_buffer;
  ExitCode program_status;
};

} // namespace gln

#endif