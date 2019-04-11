// Copyright (c) 2016 Benjamin Bäumler

#include "Entity.h"
#include "World.h"
#include <iostream>

namespace wyvern_engine {

Entity::Entity() : m_world(nullptr), m_group("none"), m_children() {}

Entity::~Entity() {
  if (m_parent) {
    m_parent->RemoveEntity(this);
  }

  m_children.clear();

  for (const auto &comp : m_components) {
    auto script = dynamic_cast<Script *>(comp.second.get());
    if (script != nullptr) {
      script->OnRemove();
    }
  }

  m_components.clear();
}

void Entity::SetGroup(std::string group) { m_group = group; }

std::string Entity::GetGroup() { return m_group; }

void Entity::Assign(EntityComponent *comp) {
  m_components[&typeid(*comp)] = std::unique_ptr<EntityComponent>(comp);
  comp->m_world = m_world;
  comp->m_owner = this;
}

void Entity::Assign(Script *script) {
  auto scr = std::unique_ptr<Script>(script);
  m_components[&typeid(*script)] = std::move(scr);
  script->m_world = m_world;
  script->m_owner = this;
  m_world->RegisterScript(script);
}

void Entity::AddEntity(Entity *entity) {
  m_children.push_back(entity);
  entity->m_parent = this;
}

void Entity::RemoveEntity(Entity *entity) {
  m_children.erase(std::remove(m_children.begin(), m_children.end(), entity),
                   m_children.end());
}

Entity *Entity::CreateEntity() {
  auto entity = m_world->CreateEntity();
  AddEntity(entity);
  return entity;
}

bool Entity::HasComponent(const std::type_index comp) const {
  for (const auto &c : m_components) {
    std::type_index test(*c.first);
    if (test == comp)
      return true;
  }
  return false;
}

Entity *Entity::GetParent() const { return m_parent; }

} // namespace wyvern_engine
