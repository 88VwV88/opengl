#include "glengine/core/textures.hpp"
#include "stb_image.hpp"
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace gln {

Texture::Texture(TextureType type) : type{type} {
  glCreateTextures(static_cast<GLenum>(type), 1, &id);
}

Texture::Texture(Texture &&other) noexcept
    : id{std::exchange(other.id, 0)},
      type{std::exchange(other.type, TextureType::InvalidTex)} {}

auto Texture::operator=(Texture &&other) noexcept -> Texture & {
  if (this != &other) {
    if (0 != id) {
      glDeleteTextures(1, &id);
    }

    id = std::exchange(other.id, 0);
    type = std::exchange(other.type, TextureType::InvalidTex);
  }
  return *this;
}

Texture::~Texture() {
  if (0 != id) {
    glDeleteTextures(1, &id);
  }
}

Texture2D::Texture2D(std::span<std::uint8_t const> image,
                     TextureInit const &init)
    : Texture{TextureType::Tex2D} {
  if (image.empty()) {
    type = TextureType::InvalidTex;
    return;
  }

  glTextureStorage2D(id, init.levels, init.internal_format, init.width,
                     init.height);
  glTextureSubImage2D(id, 0, 0, 0, init.width, init.height, init.data_format,
                      GL_UNSIGNED_BYTE, image.data());
}

auto Texture2D::from_image(const std::filesystem::path &path) -> Texture2D {
  stbi_set_flip_vertically_on_load(true);

  constexpr auto channel_count = 4;

  TextureInit init{
      .internal_format = GL_RGBA8,
      .data_format = GL_RGBA,
  };
  auto *image = stbi_load(path.c_str(), &init.width, &init.height, nullptr,
                          channel_count);
  if (nullptr == image) {
#ifndef NDEBUG
    throw std::runtime_error("Failed to load image: " + path.string());
#endif
    return Texture2D{{}, {}};
  }

  init.levels =
      (GLsizei)std::floor(std::log2(std::max(init.width, init.height))) + 1;

  Texture2D texture{std::span{image, static_cast<std::size_t>(init.width) *
                                         static_cast<std::size_t>(init.height) *
                                         channel_count},
                    init};
  texture.set_param(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture.set_param(GL_TEXTURE_WRAP_T, GL_REPEAT);
  texture.set_param(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  texture.set_param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  texture.generate_mipmap();

  stbi_image_free(image);
  return texture;
}

auto Texture2D::generate_mipmap() const -> void { glGenerateTextureMipmap(id); }

auto Texture2D::bind_unit(std::uint32_t unit) const -> void {
  glBindTextureUnit(unit, id);
}

auto Texture2D::set_param(GLenum param, std::int32_t value) const -> void {
  glTextureParameteri(id, param, value);
}

} // namespace gln
