// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_DEBUGRENDER_H
#define WYVERNENGINE_DEBUGRENDER_H

#include "RenderingSystem.h"

namespace wyvern_engine {
namespace rendering {

class DebugRender : public IRenderable {
  GLuint m_vbo;
  std::size_t m_count;
  GLenum m_mode;

public:
  DebugRender(std::vector<float> vertices, GLenum mode);

  ~DebugRender();

  void UpdateBuffer(std::vector<float> vertices);

  virtual void Render() const override;
};

} // namespace rendering
} // namespace wyvern_engine

#endif // WYVERNENGINE_DEBUGRENDER_H
