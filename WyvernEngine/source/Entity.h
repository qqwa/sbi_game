// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_ENTITY_H
#define WYVERNENGINE_ENTITY_H

#include "EntityComponent.h"
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace wyvern_engine {

class World;

class EntityComponent;

/**
 * @brief Entity is a container for other \link Entity Entities\endlink and
 * \link EntityComponent EntityComponents\endlink
 */
class Entity {
  World *m_world;
  Entity *m_parent;
  std::string m_group;
  std::vector<Entity *> m_children;
  std::unordered_map<const std::type_info *, std::unique_ptr<EntityComponent>>
      m_components;

public:
  Entity();

  ~Entity();

  /**
   * @brief SetGroup sets the group of the entity, which can be used to filter
   * entites
   */
  void SetGroup(std::string);

  /**
   *
   * @return The group of the entity
   */
  std::string GetGroup();

  /**
   * @brief Assign assigns a component to the entity
   * @param comp
   */
  void Assign(EntityComponent *comp);

  /**
   * @brief Assign assigns a script to the entity
   * @param script
   */
  void Assign(Script *script);

  /**
   * @brief AddEntity adds a child to the entity
   * @param entity
   */
  void AddEntity(Entity *entity);

  /**
   * @brief RemoveEntity removes the specified child from this entity
   * @param entity
   */
  void RemoveEntity(Entity *entity);

  /**
   * @brief CreateEntity creates a new entity, which is a child of this entity,
   * ready to use
   * @return The new child of this entity
   */
  Entity *CreateEntity();

  /**
   * @brief GetEntities filters its children for the specified components
   * @return A vector of entities with at least all of the specified components
   */
  template <typename... components> std::vector<Entity *> GetEntities();

  /**
   * @brief GetComp
   * @return The specified component or nullptr
   */
  template <typename C> C *GetComp();

  /**
   * @brief HasComponent checks if the entity contains a specific component
   * @return
   */
  template <typename C> bool HasComponent() const;

  /**
   * @brief HasComponent checks if the entity contains a specific component
   * @return
   */
  bool HasComponent(const std::type_index comp) const;

  /**
   * @brief GetParent
   * @return The Parent of this entity or nullptr if it hasn't a parent
   */
  Entity *GetParent() const;

  friend class World;
};

template <typename C> C *Entity::GetComp() {
  if (m_components.count(&typeid(C)) != 0) {
    return static_cast<C *>(m_components[&typeid(C)].get());
  } else {
    // throw exception?
    return nullptr;
  }
}

template <typename C> bool Entity::HasComponent() const {
  if (m_components.count(&typeid(C)) != 0) {
    return true;
  } else {
    return false;
  }
}

template <typename... components> std::vector<Entity *> Entity::GetEntities() {
  std::vector<std::type_index> comp_list;
  comp_list.insert(comp_list.end(), {typeid(components)...});

  std::vector<Entity *> res;

  for (auto entity : m_children) {
    bool hasComps = true;
    for (const auto &comp : comp_list) {
      if (!entity->HasComponent(comp)) {
        hasComps = false;
      }
    }

    if (hasComps) {
      res.push_back(entity);
    }
  }

  return res;
}

} // namespace wyvern_engine

#endif // WYVERNENGINE_ENTITY_H
