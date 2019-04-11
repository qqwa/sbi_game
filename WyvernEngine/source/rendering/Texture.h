// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_TEXTURE_H
#define WYVERNENGINE_TEXTURE_H

#include <GL/glew.h>
#include <memory>
#include <string>

namespace wyvern_engine {
namespace rendering {

class Texture {
  GLuint m_id;

  Texture();

public:
  typedef std::shared_ptr<Texture> Ptr;

  Texture(const std::string &path);

  ~Texture();

  static Texture *NullTexture();

  static Ptr MakeShared(const std::string &path);

  const GLuint &GetId() const;
};

} // namespace rendering
} // namespace wyvern_engine

#endif // WYVERNENGINE_TEXTURE_H
