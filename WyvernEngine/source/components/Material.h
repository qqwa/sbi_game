// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_MATERIAL_H
#define WYVERNENGINE_MATERIAL_H

#include "../EntityComponent.h"
#include "../rendering/Material.h"
#include "../rendering/Texture.h"
#include "Material.h"

namespace wyvern_engine {

class Material : public EntityComponent {
  wyvern_engine::rendering::Material::Ptr m_material;

public:
  Material(rendering::Material::Ptr mat);

  rendering::Material *GetMaterial();
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_MATERIAL_H
