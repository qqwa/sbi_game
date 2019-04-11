// Copyright (c) 2016 Benjamin Bäumler

#include "SwingScript.h"
#include <Entity.h>
#include <World.h>

SwingScript::SwingScript(float speed, bool left)
    : m_speed(speed), m_left(left) {}

void SwingScript::OnInit() {
  m_transform = m_owner->GetComp<wyvern_engine::Transform>();
}

void SwingScript::OnUpdate() {

  if (m_left) {
    m_transform->Translate(glm::vec3(-1, 0, 0) * m_speed *
                           m_world->DeltaTime());
    if (m_transform->GetPosition().x < -1.8) {
      m_transform->SetPosition(
          {-1.8, m_transform->GetPosition().y, m_transform->GetPosition().z});
      m_left = false;
    }
  } else {
    m_transform->Translate(glm::vec3(1, 0, 0) * m_speed * m_world->DeltaTime());
    if (m_transform->GetPosition().x > 1.8) {
      m_transform->SetPosition(
          {1.8, m_transform->GetPosition().y, m_transform->GetPosition().z});
      m_left = true;
    }
  }
}
