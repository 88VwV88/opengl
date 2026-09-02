#ifndef GLENGINE_PROGRAM_HPP
#define GLENGINE_PROGRAM_HPP

#include "glengine/common.hpp"
#include "glengine/utils.hpp"
#include <GL/glew.h>
#include <filesystem>

namespace gln {

struct ShaderFiles {
  std::filesystem::path const &vertex;
  std::filesystem::path const &fragment;
};

enum class ShaderType : std::uint16_t {
  VertexShader = GL_VERTEX_SHADER,
  FragmentShader = GL_FRAGMENT_SHADER,
};

struct Program {
  Program();
  GLN_DELETE_COPY_CTORS(Program);
  GLN_DEFINE_NOEXCEPT_MOVE_CTORS(Program);
  ~Program() noexcept;

  auto use() const -> void;
  auto uniform(const char *uniform, std::int32_t value) const -> void;
  auto uniform(const char *uniform, std::float_t value) const -> void;
  auto uniform(const char *uniform, glm::mat4 const &matrix) const -> void;
  auto uniform(const char *uniform, glm::vec3 const &vector) const -> void;

  [[nodiscard]] auto compile_shader_files(ShaderFiles const &files) const
      -> ExitCode;

private:
  static auto compile_shader_from_file(std::filesystem::path const &file,
                                       ShaderType shader_type) -> std::uint32_t;
  [[nodiscard]] auto check_program_link_status() const -> bool;
  static auto check_shader_compile_status(std::uint32_t shader_id) -> bool;

  std::uint32_t id;
};

} // namespace gln
#endif