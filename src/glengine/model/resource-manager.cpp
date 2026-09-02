#include "glengine/model/resource-manager.hpp"
#include "glengine/core/textures.hpp"
#include <assimp/material.h>
#include <assimp/types.h>
#include <memory>
#include <print>

namespace gln {

auto ResourceManager::process_mesh(aiMesh const *mesh)
    -> std::shared_ptr<Mesh> {
  std::vector<Vertex> vertices;
  for (std::uint32_t i = 0; i < mesh->mNumVertices; ++i) {
    glm::vec3 position{0.0f};
    if (mesh->HasPositions()) {
      position = {
          mesh->mVertices[i].x,
          mesh->mVertices[i].y,
          mesh->mVertices[i].z,
      };
    }

    glm::vec3 normal{0.0f, 1.0f, 0.0f};
    if (mesh->HasNormals()) {
      normal = {
          mesh->mNormals[i].x,
          mesh->mNormals[i].y,
          mesh->mNormals[i].z,
      };
    }

    glm::vec2 tex_coords{0.0f};
    if (mesh->HasTextureCoords(0)) {
      tex_coords = {
          mesh->mTextureCoords[0][i].x,
          mesh->mTextureCoords[0][i].y,
      };
    }

    vertices.push_back({
        .position = position,
        .normal = normal,
        .tex_coords = tex_coords,
    });
  }

  std::vector<std::uint32_t> indices;
  if (mesh->HasFaces()) {
    for (auto i = 0; i < mesh->mNumFaces; ++i) {
      indices.append_range(
          std::span{mesh->mFaces[i].mIndices, mesh->mFaces[i].mNumIndices});
    }
  }

  meshes.emplace_back(std::move(vertices), std::move(indices),
                      mesh->mMaterialIndex);
  return std::make_shared<Mesh>(meshes.back());
}

auto ResourceManager::process_material_textures(
    aiMaterial const *material, aiTextureType type,
    std::filesystem::path const &basedir)
    -> std::vector<std::shared_ptr<Texture2D>> {
  auto count = material->GetTextureCount(type);

  if (0 == count) {
    return {};
  }

  std::vector<std::shared_ptr<Texture2D>> texture_maps;

  for (auto i = 0; i < count; ++i) {
    aiString path_string;
    material->GetTexture(type, i, &path_string);

    std::filesystem::path path{basedir / path_string.C_Str()};
    std::println("Loading texture: {}", path.string());

    if (textures.contains(path)) {
      texture_maps.push_back(textures[path]);
    } else {
      textures.insert(
          {path, std::make_shared<Texture2D>(Texture2D::from_image(path))});
      texture_maps.push_back(textures[path]);
    }
  }

  return std::move(texture_maps);
}

auto ResourceManager::process_material(aiScene const &scene,
                                       aiMaterial const *material,
                                       std::filesystem::path const &basedir)
    -> std::uint32_t {
  std::println("Loading material: {}", material->GetName().C_Str());

  auto diffuse_maps =
      process_material_textures(material, aiTextureType_DIFFUSE, basedir);
  auto specular_maps =
      process_material_textures(material, aiTextureType_SPECULAR, basedir);

  auto ambient_color{DEFAULT_AMBIENT_COLOR};
  aiColor3D ai_ambient_color;
  if (material->Get(AI_MATKEY_COLOR_AMBIENT, ai_ambient_color) ==
      AI_SUCCESS) {
    ambient_color = {ai_ambient_color.r, ai_ambient_color.g,
                     ai_ambient_color.b};
  }

  auto diffuse_color{DEFAULT_DIFFUSE_COLOR};
  aiColor3D ai_diffuse_color;
  if (material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_diffuse_color) == AI_SUCCESS) {
    diffuse_color = {ai_diffuse_color.r, ai_diffuse_color.g,
                     ai_diffuse_color.b};
  }

  auto specular_color{DEFAULT_SPECULAR_COLOR};
  aiColor3D ai_specular_color;
  if (material->Get(AI_MATKEY_COLOR_SPECULAR, ai_specular_color) ==
      AI_SUCCESS) {
    specular_color = {ai_specular_color.r, ai_specular_color.g,
                      ai_specular_color.b};
  }

  auto shininess{DEFAULT_SHININESS};
  float ai_shininess;
  if (material->Get(AI_MATKEY_SHININESS, ai_shininess) == AI_SUCCESS) {
    shininess = ai_shininess;
  }

  materials.emplace_back(ambient_color, diffuse_color, specular_color,
                         std::move(diffuse_maps), std::move(specular_maps),
                         shininess);
  return materials.size() - 1;
}

auto ResourceManager::process_scene(aiScene const &scene,
                                    std::filesystem::path const &basedir)
    -> std::vector<std::shared_ptr<Mesh>> {
  std::vector<std::shared_ptr<Mesh>> meshes;

  if (scene.HasMeshes()) {
    for (std::uint32_t i = 0; i < scene.mNumMeshes; ++i) {
      meshes.emplace_back(process_mesh(scene.mMeshes[i]));
    }
  }

  if (scene.HasMaterials()) {
    for (std::uint32_t i = 0; i < scene.mNumMaterials; ++i) {
      process_material(scene, scene.mMaterials[i], basedir);
    }
  }

  return meshes;
}

auto ResourceManager::get_material(std::uint32_t index) -> Material & {
  return materials.at(index);
}

} // namespace gln
