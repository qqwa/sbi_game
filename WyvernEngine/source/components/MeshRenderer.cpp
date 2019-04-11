// Copyright (c) 2016 Benjamin Bäumler

#include "MeshRenderer.h"

namespace wyvern_engine {

void MeshRenderer::Render() const {
  glBindVertexArray(m_mesh->GetMetaData().vao);
  glDrawElements(GL_TRIANGLES, m_mesh->GetMetaData().indicesCount,
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

} // namespace wyvern_engine