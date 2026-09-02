#ifndef GLENGINE_MODEL_HPP
#define GLENGINE_MODEL_HPP

#include "glengine/core/program.hpp"
#include "glengine/geometry/vertex-array.hpp"
#include "glengine/model/mesh.hpp"
#include <filesystem>

namespace gln {

struct Scene {
  Scene(std::vector<std::shared_ptr<Mesh>> &&meshes);

  //! Draw all the meshes in the model
  auto draw(gln::Program const &program) const -> void;

  //! Create a scene from a GLTF 2.0 model file
  static auto from_gltf2(std::filesystem::path const &basedir,
                         std::string const &model_file) -> Scene;

private:
  std::vector<std::shared_ptr<Mesh>> meshes;
  VertexArray vertex_array;
};

} // namespace gln

#endif