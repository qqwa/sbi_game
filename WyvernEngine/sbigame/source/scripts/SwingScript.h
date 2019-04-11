// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_SWINGSCRIPT_H
#define WYVERNENGINE_SWINGSCRIPT_H

#include <EntityComponent.h>
#include <components/Transform.h>

class SwingScript : public wyvern_engine::Script {
  float m_x;
  float m_speed;
  bool m_left;
  wyvern_engine::Transform *m_transform;

public:
  SwingScript(float speed, bool left);

  virtual void OnInit() override;

  virtual void OnUpdate() override;
};

#endif // WYVERNENGINE_SWINGSCRIPT_H
