#ifndef GLENGINE_BUFFERS_HPP
#define GLENGINE_BUFFERS_HPP

#include "glengine/common.hpp"
#include "glengine/utils.hpp"
#include <span>

namespace gln {

struct Buffer {
  Buffer();
  GLN_DELETE_COPY_CTORS(Buffer);
  GLN_DEFINE_NOEXCEPT_MOVE_CTORS(Buffer);
  virtual ~Buffer();

  virtual auto bind() const -> void = 0;
  virtual auto unbind() const -> void = 0;

  std::uint32_t id;
};

struct VertexBuffer final : public Buffer {
  VertexBuffer(std::span<Vertex const> vertices);
  ~VertexBuffer();
  auto bind() const -> void override;
  auto unbind() const -> void override;
};

struct ElementBuffer final : public Buffer {
  ElementBuffer(std::span<std::uint32_t const> indices);
  ~ElementBuffer();
  auto bind() const -> void override;
  auto unbind() const -> void override;
  std::size_t index_count;
};

} // namespace gln

#endif