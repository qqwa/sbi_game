// Copyright (c) 2016 Benjamin Bäumler

#include "Texture.h"

//#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

namespace wyvern_engine {
namespace rendering {

Texture::Texture() : m_id(0) {}

Texture::Texture(const std::string &path) {
  int x, y, n;
  unsigned char *data = stbi_load(path.c_str(), &x, &y, &n, 0);

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() { glDeleteTextures(0, &m_id); }

Texture *Texture::NullTexture() {
  static Texture::Ptr nullTexture = std::make_shared<Texture>(Texture());
  return nullTexture.get();
}

Texture::Ptr Texture::MakeShared(const std::string &path) {
  return std::make_shared<Texture>(path);
}

const GLuint &Texture::GetId() const { return m_id; }

} // namespace rendering
} // namespace wyvern_engine