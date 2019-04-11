// Copyright (c) 2016 Benjamin Bäumler

#include "DebugRenderer.h"
#include <iostream>

namespace wyvern_engine {

DebugRenderer::DebugRenderer(std::vector<float> vertices, GLenum mode)
    : m_mode(mode) {
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  UpdateBuffer(vertices);
}

DebugRenderer::~DebugRenderer() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void DebugRenderer::UpdateBuffer(std::vector<float> vertices) {
  m_count = vertices.size() / 3;
  //        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  //        glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(float),
  //        vertices.data(), GL_DYNAMIC_DRAW);
  //
  //        glEnableVertexAttribArray(0);   //position
  //        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_count *
  //        sizeof(float), (GLvoid *) 0);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0); // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (GLvoid *)0);

  glBindVertexArray(0);
}

void DebugRenderer::Render() const {
  glLineWidth(10);
  glBindVertexArray(m_vao);
  glDrawArrays(m_mode, 0, m_count);
  glBindVertexArray(0);
}

} // namespace wyvern_engine