#ifndef GLENGINE_COMMON_HPP
#define GLENGINE_COMMON_HPP

#include <glm/glm.hpp>

namespace gln {

struct Dimensions {
  std::int32_t width;
  std::int32_t height;
};

struct SRGB {
  float red;
  float green;
  float blue;
  float alpha;
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

enum class ExitCode : std::uint8_t {
  Success = 0,
  // Platform/Window+Context
  FailedToInitalizeWindow,
  FailedToInitializeGLFW,
  FailedToInitializeOpenGL,
  // Core/Program
  FailedToLinkProgram,
  FailedToReadShaderSource,
  FailedToCompileVertexShader,
  FailedToCompileFragmentShader,
  // Core/Texure
  FailedToReadImage,
};

} // namespace gln

#endif