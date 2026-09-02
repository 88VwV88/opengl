#ifndef GLENGINE_STORAGE_MANAGER_HPP
#define GLENGINE_STORAGE_MANAGER_HPP

#include "glengine/geometry/buffers.hpp"
#include <memory>
#include <vector>

namespace gln {

struct StorageManager {
  static auto get_vertex_buffer(std::uint32_t index) -> VertexBuffer &;
  static auto get_element_buffer(std::uint32_t index) -> ElementBuffer &;

  static auto allocate_vertex_buffer(std::vector<Vertex> &&vertices)
      -> std::uint32_t;
  static auto allocate_element_buffer(std::vector<std::uint32_t> &&indices)
      -> std::uint32_t;

private:
  inline static std::vector<std::shared_ptr<Buffer>> buffers;
};

} // namespace gln

#endif