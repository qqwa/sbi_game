// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_MESHCOMPONENT_H
#define WYVERNENGINE_MESHCOMPONENT_H

#include "../EntityComponent.h"
#include "../rendering/Mesh.h"
#include "../rendering/RenderingSystem.h"

namespace wyvern_engine {

class MeshRenderer : public EntityComponent, public rendering::IRenderable {
  rendering::Mesh::Ptr m_mesh;

public:
  MeshRenderer(rendering::Mesh::Ptr mesh) : m_mesh(mesh) {}

  void Render() const;
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_MESHCOMPONENT_H
