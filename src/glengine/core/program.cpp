#include <fstream>
#include <glengine/core/program.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace gln {

Program::Program() : id{glCreateProgram()} {}

Program::~Program() noexcept {
  if (0 != id) {
    glDeleteProgram(id);
  }
}

auto Program::use() const -> void { glUseProgram(id); }

auto Program::uniform(const char *uniform, std::int32_t value) const -> void {
  glUniform1i(glGetUniformLocation(id, uniform), value);
}

auto Program::uniform(const char *uniform, std::float_t value) const -> void {
  glUniform1f(glGetUniformLocation(id, uniform), value);
}

auto Program::uniform(const char *uniform, glm::mat4 const &matrix) const
    -> void {
  glUniformMatrix4fv(glGetUniformLocation(id, uniform), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

auto Program::uniform(const char *uniform, glm::vec3 const &vector) const
    -> void {
  glUniform3fv(glGetUniformLocation(id, uniform), 1, glm::value_ptr(vector));
}

auto Program::compile_shader_files(ShaderFiles const &file) const -> ExitCode {
  std::uint32_t vertex{
      compile_shader_from_file(file.vertex, ShaderType::VertexShader)};
  if (0 == vertex) {
    return ExitCode::FailedToCompileVertexShader;
  }

  std::uint32_t fragment{
      compile_shader_from_file(file.fragment, ShaderType::FragmentShader)};
  if (0 == fragment) {
    glDeleteShader(vertex);
    return ExitCode::FailedToCompileFragmentShader;
  }

  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);

  if (!check_program_link_status()) {
    return ExitCode::FailedToLinkProgram;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return ExitCode::Success;
}

auto Program::compile_shader_from_file(std::filesystem::path const &file,
                                       ShaderType shader_type)
    -> std::uint32_t {
  std::ifstream stream;

#ifndef NDEBUG
  stream.exceptions(std::ios::badbit | std::ios::failbit);
#endif

  stream.open(file, std::ios::ate | std::ios::binary);
  if (!stream.is_open()) {
#ifndef NDEBUG
    throw std::runtime_error("Failed to open file: " + file.string());
#endif
    return 0;
  }

  std::string contents(stream.tellg(), '\0');
  stream.seekg(0, std::ios::beg);
  stream.read(contents.data(), static_cast<std::streamsize>(contents.size()));
  stream.close();

  std::uint32_t shader = glCreateShader(static_cast<GLenum>(shader_type));
  auto const *shader_source = contents.c_str();
  glShaderSource(shader, 1, &shader_source, nullptr);
  glCompileShader(shader);

  if (!check_shader_compile_status(shader)) {
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

auto Program::check_shader_compile_status(std::uint32_t shader_id) -> bool {
  GLint param = GL_TRUE;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &param);
  if (GL_TRUE == param) {
    return true;
  }

#ifndef NDEBUG
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &param);
  std::string info_log(param, '\0');
  glGetShaderInfoLog(shader_id, param, nullptr, info_log.data());

  throw std::runtime_error("Failed to compile shader: " + info_log);
#endif

  return false;
}

auto Program::check_program_link_status() const -> bool {
  GLint param = GL_TRUE;
  glGetProgramiv(id, GL_LINK_STATUS, &param);
  if (GL_TRUE == param) {
    return true;
  }

#ifndef NDEBUG
  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &param);
  std::string info_log(param, '\0');
  glGetProgramInfoLog(id, param, nullptr, info_log.data());

  throw std::runtime_error("Failed to link program: " + info_log);
#endif

  return false;
}

} // namespace gln