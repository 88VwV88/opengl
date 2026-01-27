#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

#include "shader.hpp"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
  // bone indexes which will influence this vertex
  int m_BoneIDs[MAX_BONE_INFLUENCE];
  // weights for each bone
  float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
  unsigned int id;
  std::string path;
  std::string type;
};

class Mesh {
public:
  // mesh data
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int VAO;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures)
      : vertices(std::move(vertices)), indices(std::move(indices)),
        textures(std::move(textures)), VAO(0), VBO(0), EBO(0) {
    setupMesh();
  }

  // Delete copy constructor and copy assignment (prevent accidental copies)
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  // Move constructor
  Mesh(Mesh &&other) noexcept
      : vertices(std::move(other.vertices)), indices(std::move(other.indices)),
        textures(std::move(other.textures)), VAO(other.VAO), VBO(other.VBO),
        EBO(other.EBO) {
    // Reset the source object's handles so its destructor won't delete our
    // resources
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
  }

  // Move assignment operator
  Mesh &operator=(Mesh &&other) noexcept {
    if (this != &other) {
      // Clean up existing resources
      if (VAO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
      }

      // Move data
      vertices = std::move(other.vertices);
      indices = std::move(other.indices);
      textures = std::move(other.textures);
      VAO = other.VAO;
      VBO = other.VBO;
      EBO = other.EBO;

      // Reset source handles
      other.VAO = 0;
      other.VBO = 0;
      other.EBO = 0;
    }
    return *this;
  }

  ~Mesh() {
    if (VAO != 0) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);

      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);
    }
  }

  void Draw(Shader &shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); ++i) {
      glActiveTexture(GL_TEXTURE0 + i);

      std::string number;
      if (textures[i].type == "texture_diffuse")
        number = std::to_string(diffuseNr++);
      else if (textures[i].type == "texture_specular")
        number = std::to_string(specularNr++);
      else if (textures[i].type == "texture_normal")
        number = std::to_string(normalNr++);
      else if (textures[i].type == "texture_height")
        number = std::to_string(heightNr++);

      shader.setInt((textures[i].type + number).c_str(), i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                   GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
  }

private:
  // render data
  unsigned int VBO, EBO;

  // initialize all buffer objects/arrays
  void setupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex),
                           (void *)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
  }
};

#endif