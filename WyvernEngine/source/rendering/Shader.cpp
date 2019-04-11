// Copyright (c) 2016 Benjamin Bäumler

#include "Shader.h"
#include "../Helper.h"
#include <iostream>

namespace wyvern_engine {
namespace rendering {

Shader::Shader(std::string vertex_path, std::string fragment_path) {
  GLint success;
  GLchar infoLog[512];

  // compile vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertexSource = helper::loadFile(vertex_path);
  glShaderSource(vertexShader, 1, (const GLchar **)&vertexSource, nullptr);
  glCompileShader(vertexShader);
  // error check
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << vertex_path
              << "\n"
              << infoLog << std::endl;
  }

  // compile fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragmentSource = helper::loadFile(fragment_path);
  glShaderSource(fragmentShader, 1, (const GLchar **)&fragmentSource, nullptr);
  glCompileShader(fragmentShader);
  // error check
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: "
              << fragment_path << "\n"
              << infoLog << std::endl;
  }

  // create program
  m_program = glCreateProgram();
  glAttachShader(m_program, vertexShader);
  glAttachShader(m_program, fragmentShader);
  glLinkProgram(m_program);
  // error check
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(m_program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  Bind();
}

Shader::~Shader() { glDeleteProgram(m_program); }

void Shader::Bind() const { glUseProgram(m_program); }

GLint Shader::GetUniformLocation(const std::string name) const {
  return glGetUniformLocation(m_program, name.c_str());
}

} // namespace rendering
} // namespace wyvern_engine