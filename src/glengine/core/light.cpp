#include "glengine/core/light.hpp"
#include "glengine/geometry/buffers.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

namespace gln {

Light::Light(ShaderFiles const &files)
    : vertex_buffer{LIGHT_VERTICES}, element_buffer{LIGHT_INDICES} {
  program_status = program.compile_shader_files(files);
  if (ExitCode::Success != program_status) {
    return;
  }

  vertex_array.link_vertex_attributes();
  vertex_array.attach_buffer(vertex_buffer);
  vertex_array.attach_buffer(element_buffer);
}

auto Light::draw(LightDrawMatrices const &params) const -> void {
  program.use();

  program.uniform("uModel", params.model);
  program.uniform("uView", params.view);
  program.uniform("uProjection", params.projection);
  program.uniform("uLightColor", LIGHT_COLOR);

  vertex_array.bind();
  vertex_buffer.bind();
  element_buffer.bind();

  glDrawElements(GL_TRIANGLES, LIGHT_INDICES.size(), GL_UNSIGNED_INT, nullptr);
}

auto Light::status() const -> ExitCode { return program_status; }

} // namespace gln