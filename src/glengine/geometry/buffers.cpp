#include "glengine/geometry/buffers.hpp"
#include <GL/glew.h>

namespace gln {

VertexBuffer::VertexBuffer(std::span<Vertex const> vertices) {
  glNamedBufferStorage(id, static_cast<GLsizeiptr>(vertices.size_bytes()),
                       vertices.data(), 0);
}

VertexBuffer::~VertexBuffer() { unbind(); }

auto VertexBuffer::bind() const -> void { glBindBuffer(GL_ARRAY_BUFFER, id); }

auto VertexBuffer::unbind() const -> void { glBindBuffer(GL_ARRAY_BUFFER, 0); }

ElementBuffer::ElementBuffer(std::span<std::uint32_t const> indices)
    : index_count{indices.size()} {
  glNamedBufferStorage(id, static_cast<GLsizeiptr>(indices.size_bytes()),
                       indices.data(), 0);
}

ElementBuffer::~ElementBuffer() { unbind(); }

auto ElementBuffer::bind() const -> void {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

auto ElementBuffer::unbind() const -> void {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Buffer::Buffer() { glCreateBuffers(1, &id); }

Buffer::~Buffer() {
  if (0 != id) {
    glDeleteBuffers(1, &id);
  }
}

} // namespace gln
