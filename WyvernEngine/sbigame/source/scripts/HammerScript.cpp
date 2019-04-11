// Copyright (c) 2016 Benjamin Bäumler

#include "HammerScript.h"
#include <Entity.h>
#include <World.h>
#include <components/Transform.h>

void HammerScript::OnUpdate() {
  m_owner->GetComp<wyvern_engine::Transform>()->Rotate(
      45 * m_world->DeltaTime(), {0, 1, 0});
}
