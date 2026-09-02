#include "glengine/geometry/vertex-array.hpp"
#include "glengine/common.hpp"
#include "glengine/geometry/buffers.hpp"
#include <GL/glew.h>
#include <cstddef>

namespace gln {

VertexArray::VertexArray() {
  glCreateVertexArrays(1, &id);
  link_vertex_attributes();
}

auto VertexArray::attach_buffer(VertexBuffer const &buffer) const -> void {
  buffer.bind();
  glVertexArrayVertexBuffer(id, 0, buffer.id, 0,
                            static_cast<GLsizei>(sizeof(Vertex)));
}

auto VertexArray::attach_buffer(ElementBuffer const &buffer) const -> void {
  buffer.bind();
  glVertexArrayElementBuffer(id, buffer.id);
}

auto VertexArray::link_vertex_attributes() const -> void {
  link_attribute({
      .index = 0,
      .count = 3,
      .offset = static_cast<std::uint32_t>(offsetof(Vertex, position)),
  });
  link_attribute({
      .index = 1,
      .count = 3,
      .offset = static_cast<std::uint32_t>(offsetof(Vertex, normal)),
  });
  link_attribute({
      .index = 2,
      .count = 2,
      .offset = static_cast<std::uint32_t>(offsetof(Vertex, tex_coords)),
  });
}

auto VertexArray::link_attribute(AttribInfo const &info) const -> void {
  glVertexArrayAttribFormat(id, info.index, info.count, GL_FLOAT, GL_FALSE,
                            info.offset);
  glVertexArrayAttribBinding(id, info.index, 0);
  glEnableVertexArrayAttrib(id, info.index);
}

auto VertexArray::bind() const -> void { glBindVertexArray(id); }

auto VertexArray::unbind() const -> void { glBindVertexArray(0); }

VertexArray::~VertexArray() {
  if (0 != id) {
    unbind();
    glDeleteVertexArrays(1, &id);
  }
}

} // namespace gln
