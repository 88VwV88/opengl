#pragma once

#include <glad/glad.h>
#include <span>

struct VBO {
  unsigned int ID;
  void bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
  void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
  VBO(std::span<const float> vertices) {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);
    unbind();
  }
  ~VBO() {
    unbind();
    glDeleteBuffers(1, &ID);
  }
};

struct EBO {
  unsigned int ID;
  void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
  void unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
  EBO(std::span<const unsigned int> indices) {
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);
    unbind();
  }
  ~EBO() {
    unbind();
    glDeleteBuffers(1, &ID);
  }
};

struct VAO {
  unsigned int ID;
  void bind() { glBindVertexArray(ID); }
  void unbind() { glBindVertexArray(0); }
  VAO() { glGenVertexArrays(1, &ID); }
  void linkAttrib(VBO &vbo, int layout, int count, unsigned int stride,
                  void *offset) {
    vbo.bind();
    glVertexAttribPointer(layout, count, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    vbo.unbind();
  }
  ~VAO() {
    unbind();
    glDeleteVertexArrays(1, &ID);
  }
};