// Copyright (c) 2016 Benjamin Bäumler

#include "Camera.h"
#include "../Entity.h"
#include "Transform.h"

namespace wyvern_engine {

Camera::Camera(glm::mat4 projection) : m_projection(projection) {}

Camera::~Camera() {}

const glm::mat4 &Camera::GetProjection() const { return m_projection; }

glm::mat4 Camera::GetTransform() const {
  auto trans = m_owner->GetComp<Transform>()->GetMatrix();
  return glm::inverse(trans);
}

} // namespace wyvern_engine