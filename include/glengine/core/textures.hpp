#ifndef GLENGINE_TEXTURES_HPP
#define GLENGINE_TEXTURES_HPP

#include "glengine/utils.hpp"
#include <GL/glew.h>
#include <cstdint>
#include <filesystem>
#include <span>

namespace gln {

enum class TextureType : std::uint16_t {
  InvalidTex,
  Tex1D = GL_TEXTURE_1D,
  Tex2D = GL_TEXTURE_2D,
  Tex3D = GL_TEXTURE_3D,
  CubeMap = GL_TEXTURE_CUBE_MAP,
};

struct Texture {
  Texture(TextureType type);
  GLN_DELETE_COPY_CTORS(Texture);
  Texture(Texture &&other) noexcept;
  auto operator=(Texture &&other) noexcept -> Texture &;
  ~Texture();

  std::uint32_t id;
  TextureType type;
};

struct TextureInit {
  std::int32_t levels;
  GLenum internal_format;
  GLenum data_format;
  std::int32_t width;
  std::int32_t height;
};

struct Texture2D final : private Texture {
  Texture2D(std::span<std::uint8_t const> image, TextureInit const &info);
  static auto from_image(std::filesystem::path const &path) -> Texture2D;

  auto generate_mipmap() const -> void;
  auto bind_unit(std::uint32_t unit) const -> void;
  auto set_param(GLenum param, std::int32_t value) const -> void;
};

} // namespace gln

#endif
