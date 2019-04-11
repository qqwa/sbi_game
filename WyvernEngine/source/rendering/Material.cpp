// Copyright (c) 2016 Benjamin Bäumler

#include "Material.h"
#include <glm/vec3.hpp>

namespace wyvern_engine {
namespace rendering {

Material::Material(Shader::Ptr shader)
    : m_color({0.0f, 0.0f, 0.0f, 1.0f}), m_shader(shader) {}

glm::vec4 Material::GetColor() const { return m_color; }

Texture *Material::GetTexture(const TextureType &type) {
  if (m_textures.count(type) != 0) {
    return m_textures[type].get();
  } else {
    return Texture::NullTexture();
  }
}

Shader *Material::GetShader() { return m_shader.get(); }

void Material::SetColor(const glm::vec4 &color) { m_color = color; }

void Material::SetColor(const glm::vec3 &color) {
  SetColor(glm::vec4{color, m_color.w});
}

void Material::SetTexture(const Material::TextureType &type,
                          Texture::Ptr texture) {
  m_textures[type] = texture;
}

void Material::RemoveTexture(const Material::TextureType &type) {
  m_textures.erase(type);
}

} // namespace rendering
} // namespace wyvern_engine