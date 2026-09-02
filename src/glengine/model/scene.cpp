#include "glengine/model/scene.hpp"
#include "glengine/model/resource-manager.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace gln {

Scene::Scene(std::vector<std::shared_ptr<Mesh>> &&meshes) : meshes{meshes} {}

auto Scene::from_gltf2(std::filesystem::path const &basedir,
                       std::string const &model_file) -> Scene {
  Assimp::Importer importer;
  auto import_flags = aiProcess_FlipUVs | aiProcess_Triangulate |
                      aiProcess_GenNormals | aiProcess_PreTransformVertices |
                      aiProcess_ValidateDataStructure;

  auto const *scene =
      importer.ReadFile(basedir.string() + model_file, import_flags);
  return ResourceManager::process_scene(*scene, basedir);
}

auto Scene::draw(Program const &program) const -> void {
  for (auto const &mesh : meshes) {
    mesh->draw(program, vertex_array);
  }
}

} // namespace gln
