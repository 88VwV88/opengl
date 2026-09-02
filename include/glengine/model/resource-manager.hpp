#ifndef MATERIAL_MANAGER_HPP
#define MATERIAL_MANAGER_HPP

#include "glengine/core/textures.hpp"
#include "glengine/model/mesh.hpp"
#include <assimp/scene.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include <memory>

namespace gln {

struct ResourceManager {
  static auto process_scene(aiScene const &scene,
                            std::filesystem::path const &basedir)
      -> std::vector<std::shared_ptr<Mesh>>;
  static auto get_material(std::uint32_t index) -> Material &;

private:
  static auto process_material(aiScene const &scene, aiMaterial const *material,
                               std::filesystem::path const &basedir)
      -> std::uint32_t;
  static auto process_material_textures(aiMaterial const *material,
                                        aiTextureType type,
                                        std::filesystem::path const &basedir)
      -> std::vector<std::shared_ptr<Texture2D>>;
  static auto process_mesh(aiMesh const *mesh) -> std::shared_ptr<Mesh>;

  inline static std::vector<Mesh> meshes;
  inline static std::vector<Material> materials;
  inline static std::map<std::filesystem::path, std::shared_ptr<Texture2D>>
      textures;
};

} // namespace gln

#endif