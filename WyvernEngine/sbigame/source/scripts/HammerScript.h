// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_HAMMERSCRIPT_H
#define WYVERNENGINE_HAMMERSCRIPT_H

#include <EntityComponent.h>

class HammerScript : public wyvern_engine::Script {

public:
  virtual void OnUpdate() override;
};

#endif // WYVERNENGINE_HAMMERSCRIPT_H
