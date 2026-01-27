#ifndef MODEL_HPP
#define MODEL_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include "stb_image.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const std::string &path, bool gamma = false);

class Model {
public:
  Model(const char *path) { loadModel(path); }
  void Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); ++i)
      meshes[i].Draw(shader);
  }
  ~Model() {
    for (auto &[_, texture] : textures_loaded)
      glDeleteTextures(1, &texture.id);
  }

private:
  std::vector<Mesh> meshes;
  std::string directory;
  std::unordered_map<std::string, Texture> textures_loaded;

  void loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
  }
  void processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
      processNode(node->mChildren[i], scene);
    }
  }
  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // process vertex positions, normals, and texture coordinates
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex;
      vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                  mesh->mVertices[i].z);
      vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);
      if (mesh->mTextureCoords[0]) {
        vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                     mesh->mTextureCoords[0][i].y);
      } else {
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      }
      vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; ++j) {
        indices.push_back(face.mIndices[j]);
      }
    }
    // process material
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures);
  }
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);
      std::string path = directory + '/' + str.C_Str();

      if (textures_loaded.contains(path)) {
        textures.push_back(textures_loaded[path]);
      } else {
        std::cout << "Texture loading at path: " << str.C_Str() << std::endl;

        Texture texture{
            .id = TextureFromFile(path), .path = path, .type = typeName};
        textures_loaded[path] = texture;
        textures.push_back(texture);
      }
    }
    return textures;
  }
};

unsigned int TextureFromFile(const std::string &path,
                             [[maybe_unused]] bool gamma) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;
    else
      std::cerr << "Invalid image format for image: " << path << std::endl;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  } else {
    std::cerr << "Texture failed to load at path: " << path << std::endl;
  }
  stbi_image_free(data);

  return textureID;
}

#endif