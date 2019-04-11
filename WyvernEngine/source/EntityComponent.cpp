// Copyright (c) 2016 Benjamin Bäumler

#include "EntityComponent.h"
#include "World.h"

namespace wyvern_engine {

EntityComponent::~EntityComponent() {}

Script::~Script() { m_world->RemoveScript(this); }

} // namespace wyvern_engine