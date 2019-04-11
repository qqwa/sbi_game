// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_RENDERING_MATERIAL_H
#define WYVERNENGINE_RENDERING_MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <unordered_map>

namespace wyvern_engine {
namespace rendering {

/**
 * @brief The Material class contains several information's for the shader to
 * draw something.
 */
class Material {
public:
  enum class TextureType { DIFFUSE, NORMAL, SPECULAR };

private:
  glm::vec4 m_color;
  Shader::Ptr m_shader;
  std::unordered_map<const Material::TextureType, Texture::Ptr> m_textures;

public:
  typedef std::shared_ptr<Material> Ptr;

  Material(Shader::Ptr shader);

  glm::vec4 GetColor() const;

  Texture *GetTexture(const TextureType &type);

  Shader *GetShader();

  void SetColor(const glm::vec4 &color);

  void SetColor(const glm::vec3 &color);

  void SetTexture(const TextureType &type, Texture::Ptr texture);

  void RemoveTexture(const TextureType &type);
};

} // namespace rendering
} // namespace wyvern_engine

#endif // WYVERNENGINE_MATERIAL_H
