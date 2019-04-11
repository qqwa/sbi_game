// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_DEBUGRENDERER_H
#define WYVERNENGINE_DEBUGRENDERER_H

#include "../EntityComponent.h"
#include "../rendering/DebugRender.h"
#include "../rendering/RenderingSystem.h"

namespace wyvern_engine {

class DebugRenderer : public EntityComponent, public rendering::IRenderable {
  GLuint m_vbo;
  GLuint m_vao;
  std::size_t m_count;
  GLenum m_mode;

public:
  DebugRenderer(std::vector<float>, GLenum);

  ~DebugRenderer();

  void UpdateBuffer(std::vector<float> vertices);

  void Render() const;
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_DEBUGRENDERER_H
