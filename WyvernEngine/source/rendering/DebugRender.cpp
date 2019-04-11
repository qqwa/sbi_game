// Copyright (c) 2016 Benjamin Bäumler

#include "DebugRender.h"

namespace wyvern_engine {
namespace rendering {

DebugRender::DebugRender(std::vector<float> vertices, GLenum mode)
    : m_mode(mode) {
  glGenBuffers(1, &m_vbo);
  UpdateBuffer(vertices);
}

DebugRender::~DebugRender() { glDeleteBuffers(1, &m_vbo); }

void DebugRender::UpdateBuffer(std::vector<float> vertices) {
  m_count = vertices.size();
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(float), vertices.data(),
               GL_DYNAMIC_DRAW);
}

void DebugRender::Render() const { glDrawArrays(m_mode, 0, m_count); }

} // namespace rendering
} // namespace wyvern_engine