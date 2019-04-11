// Copyright (c) 2016 Benjamin Bäumler

#include "Material.h"

namespace wyvern_engine {

Material::Material(rendering::Material::Ptr mat) : m_material(mat) {}

rendering::Material *Material::GetMaterial() { return m_material.get(); }

} // namespace wyvern_engine