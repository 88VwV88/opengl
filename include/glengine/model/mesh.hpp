#ifndef GLENGINE_MESH_HPP
#define GLENGINE_MESH_HPP

#include "glengine/common.hpp"
#include "glengine/core/program.hpp"
#include "glengine/core/textures.hpp"
#include "glengine/geometry/vertex-array.hpp"
#include <memory>

namespace gln {

static constexpr glm::vec3 DEFAULT_AMBIENT_COLOR{0.2f, 0.2f, 0.2f};
static constexpr glm::vec3 DEFAULT_DIFFUSE_COLOR{0.8f, 0.8f, 0.8f};
static constexpr glm::vec3 DEFAULT_SPECULAR_COLOR{0.5f, 0.5f, 0.5f};
static constexpr float DEFAULT_SHININESS{32.0f};

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  std::vector<std::shared_ptr<Texture2D>> diffuse_maps;
  std::vector<std::shared_ptr<Texture2D>> specular_maps;

  float shininess;
};

struct Mesh {
  Mesh(std::vector<Vertex> &&vertices, std::vector<std::uint32_t> &&indices,
       std::uint32_t material);
  //! Draw the mesh vertices, indices and materials
  auto draw(Program const &program, VertexArray const &vertex_array) const
      -> void;

private:
  auto bind_material(Program const &program) const -> void;

  std::uint32_t vertex_buffer;
  std::uint32_t element_buffer;
  std::uint32_t material;
};

} // namespace gln

#endif
