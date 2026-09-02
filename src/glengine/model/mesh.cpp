#include "glengine/model/mesh.hpp"
#include "glengine/core/program.hpp"
#include "glengine/geometry/storage-manager.hpp"
#include "glengine/geometry/vertex-array.hpp"
#include "glengine/model/resource-manager.hpp"

namespace gln {

Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<std::uint32_t> &&indices,
           std::uint32_t material)
    : vertex_buffer(
          StorageManager::allocate_vertex_buffer(std::move(vertices))),
      element_buffer(
          StorageManager::allocate_element_buffer(std::move(indices))),
      material{material} {}

auto Mesh::draw(Program const &program, VertexArray const &vertex_array) const
    -> void {
  auto const &vbo = StorageManager::get_vertex_buffer(vertex_buffer);
  auto const &ebo = StorageManager::get_element_buffer(element_buffer);

  vertex_array.bind();
  vertex_array.attach_buffer(vbo);
  vertex_array.attach_buffer(ebo);

  program.use();
  bind_material(program);

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ebo.index_count),
                 GL_UNSIGNED_INT, nullptr);
}

auto Mesh::bind_material(Program const &program) const -> void {
  auto const &material = ResourceManager::get_material(this->material);

  program.use();
  program.uniform("uMaterial.ambient", material.ambient);
  program.uniform("uMaterial.diffuse", material.diffuse);
  program.uniform("uMaterial.specular", material.specular);

  program.uniform("uMaterial.useDiffuseMap",
                  static_cast<std::int32_t>(!material.diffuse_maps.empty()));
  program.uniform("uMaterial.useSpecularMap",
                  static_cast<std::int32_t>(!material.specular_maps.empty()));

  if (!material.diffuse_maps.empty()) {
    material.diffuse_maps.front()->bind_unit(0);
  }
  program.uniform("uMaterial.diffuseMap", 0);

  if (!material.specular_maps.empty()) {
    material.specular_maps.front()->bind_unit(1);
  }
  program.uniform("uMaterial.specularMap", 1);

  program.uniform("uMaterial.shininess", material.shininess);
}

} // namespace gln
