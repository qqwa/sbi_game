// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_RENDERINGSYSTEM_H
#define WYVERNENGINE_RENDERINGSYSTEM_H

#include "../Entity.h"
#include "Shader.h"

namespace wyvern_engine {
namespace rendering {

/**
 *
 */
class IRenderable {
public:
  virtual ~IRenderable() {}

  virtual void Render() const = 0;
};

class RenderingSystem {
  Entity *m_camera;
  World *m_world;

public:
  RenderingSystem(World *world, Entity *camera);

  ~RenderingSystem();

  void Render();
};

} // namespace rendering
} // namespace wyvern_engine

#endif // WYVERNENGINE_RENDERINGSYSTEM_H
