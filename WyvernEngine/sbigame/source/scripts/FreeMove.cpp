// Copyright (c) 2016 Benjamin Bäumler

#include "FreeMove.h"
#include "../components/SegmentRef.h"
#include <components/Material.h>

void FreeMove::OnUpdate() {
  auto trans = m_owner->GetComp<Transform>();

  auto x = -m_world->GetInput()->GetAxis(Input::Axis::HORIZONTAL);
  auto y = m_world->GetInput()->GetAxis(Input::Axis::VERTICAL);

  if (m_world->GetInput()->GetAction(Input::Action::SHOT)) {
    speed += m_world->DeltaTime() * 2;
    if (speed > max_speed) {
      speed = max_speed;
    }
  }

  if (m_world->GetInput()->GetAction(Input::Action::SHIFT)) {
    speed -= m_world->DeltaTime() * 2;
    if (speed < min_speed) {
      speed = min_speed;
    }
  }

  auto tunnel = m_world->GetEntities("tunnel")[0];
  auto segRef = tunnel->GetComp<SegmentRef>();

  auto segments = segRef->GetSegments(0);
  auto mat = segments[0]
                 ->GetEntities<wyvern_engine::Material>()[0]
                 ->GetComp<wyvern_engine::Material>();
  mat->GetMaterial()->SetColor(
      glm::vec3(-0.01 * speed, 0.005 * speed, 0.01 * speed));

  auto forward = trans->GetForward();

  trans->Translate(forward * speed * m_world->DeltaTime());

  trans->Rotate(x * m_world->DeltaTime() * rotation_speed, glm::vec3(0, 1, 0));
  trans->Rotate(y * m_world->DeltaTime() * rotation_speed, glm::vec3(1, 0, 0));
}
