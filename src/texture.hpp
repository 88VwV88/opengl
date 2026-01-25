#pragma once

#include "shader.hpp"
#include "stb_image.hpp"

#include <glad/glad.h>
#include <iostream>

struct Texture {
  unsigned int ID;
  unsigned int unit;

  Texture(const char *filename, unsigned int unit) : unit(unit) {
    glGenTextures(1, &ID);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) {
      GLenum format;
      switch (nrChannels) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        throw std::runtime_error("Invalid image format");
      }

      glBindTexture(GL_TEXTURE_2D, ID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cerr << "Texture failed to load at path: " << filename << std::endl;
    }
    stbi_image_free(data);
  }
  void bind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
  }
  void setTexUnit(Shader &program, const std::string &name) {
    program.setInt(name.c_str(), unit);
  }
  ~Texture() { glDeleteTextures(1, &ID); }
};