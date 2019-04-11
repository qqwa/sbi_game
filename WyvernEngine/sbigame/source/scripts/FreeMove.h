// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_FREEMOVE_H
#define WYVERNENGINE_FREEMOVE_H

#include <EntityComponent.h>
#include <Input.h>
#include <World.h>
#include <components/Transform.h>
#include <glm/glm.hpp>

using wyvern_engine::EntityComponent;
using wyvern_engine::Input;
using wyvern_engine::Script;
using wyvern_engine::Transform;

class FreeMove : public Script {
  float min_speed;
  float max_speed;
  float rotation_speed;

public:
  float speed;

  FreeMove(float speed, float mouse_sensitivity)
      : speed(speed), min_speed(speed / 5), max_speed(speed * 5),
        rotation_speed(mouse_sensitivity) {}

  virtual void OnUpdate() override;
};

#endif // WYVERNENGINE_FREEMOVE_H
