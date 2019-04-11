// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_CAMERA_H
#define WYVERNENGINE_CAMERA_H

#include "../EntityComponent.h"
#include <glm/detail/type_mat4x4.hpp>

namespace wyvern_engine {

/**
 * @brief Camera component will be used by the RenderingSystem
 */
class Camera : public EntityComponent {
  glm::mat4 m_projection;

public:
  explicit Camera(glm::mat4 projection);

  const glm::mat4 &GetProjection() const;

  glm::mat4 GetTransform() const;

  ~Camera();
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_CAMERA_H
