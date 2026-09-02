#ifndef GLENGINE_VERTEX_ARRAY_HPP
#define GLENGINE_VERTEX_ARRAY_HPP

#include "glengine/common.hpp"
#include "glengine/geometry/buffers.hpp"
#include <span>

namespace gln {

struct AttribInfo {
  std::uint32_t index;
  std::int32_t count;
  std::uint32_t offset;
};

struct VertexArray {
  VertexArray();
  ~VertexArray();

  auto bind() const -> void;
  auto unbind() const -> void;
  auto attach_buffer(VertexBuffer const &buffer) const -> void;
  auto attach_buffer(ElementBuffer const &buffer) const -> void;
  auto link_vertex_attributes() const -> void;

private:
  auto link_attribute(AttribInfo const &info) const -> void;

  std::uint32_t id;
};

} // namespace gln

#endif
