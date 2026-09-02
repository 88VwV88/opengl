#include "glengine/geometry/storage-manager.hpp"
#include "glengine/geometry/buffers.hpp"
#include <memory>

namespace gln {

auto StorageManager::get_vertex_buffer(std::uint32_t index) -> VertexBuffer & {
  return *std::dynamic_pointer_cast<VertexBuffer>(buffers.at(index));
}

auto StorageManager::get_element_buffer(std::uint32_t index)
    -> ElementBuffer & {
  return *std::dynamic_pointer_cast<ElementBuffer>(buffers.at(index));
}

auto StorageManager::allocate_vertex_buffer(std::vector<Vertex> &&vertices)
    -> std::uint32_t {
  auto vertex_buffer = std::make_shared<VertexBuffer>(vertices);
  buffers.emplace_back(std::move(vertex_buffer));
  return buffers.size() - 1;
}

auto StorageManager::allocate_element_buffer(
    std::vector<std::uint32_t> &&indices) -> std::uint32_t {
  auto element_buffer = std::make_shared<ElementBuffer>(indices);
  buffers.emplace_back(std::move(element_buffer));
  return buffers.size() - 1;
}

} // namespace gln