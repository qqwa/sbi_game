// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_SHADER_H
#define WYVERNENGINE_SHADER_H

#include <GL/glew.h>
#include <memory>
#include <string>

namespace wyvern_engine {
namespace rendering {

/**
 * @brief Shader will be used to load and use shaders for opengl
 */
class Shader {
  GLuint m_program;

public:
  typedef std::shared_ptr<Shader> Ptr;

  Shader(std::string vertex_path, std::string fragment_path);

  ~Shader();

  void Bind() const;

  GLint GetUniformLocation(const std::string name) const;
};

} // namespace rendering
} // namespace wyvern_engine

#endif // WYVERNENGINE_SHADER_H
