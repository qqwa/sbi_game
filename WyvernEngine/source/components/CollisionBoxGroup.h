// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_COLLISIONBOXGROUP_H
#define WYVERNENGINE_COLLISIONBOXGROUP_H

#include "../EntityComponent.h"
#include <bits/unique_ptr.h>
#include <functional>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace wyvern_engine {

class CollisionBox;

class CollisionBoxGroup : public EntityComponent {
  std::vector<std::unique_ptr<CollisionBox>> m_children;
  glm::vec3 m_position;
  glm::quat m_orientation;
  std::string m_kind;

public:
  CollisionBoxGroup(std::string kind);

  ~CollisionBoxGroup();

  void AddCollisionBox(glm::vec3 a, glm::vec3 b);

  bool CheckCollision(CollisionBoxGroup *targetGroup, std::string &kind);

  bool CheckCollision(CollisionBox *target, std::string &kind);

  friend class CollisionBox;
};

class CollisionBox {
  CollisionBoxGroup *m_parent;
  glm::vec3 m_a;
  glm::vec3 m_b;

public:
  CollisionBox(glm::vec3 a, glm::vec3 b, CollisionBoxGroup *parent);

  ~CollisionBox();

  bool CheckCollision(CollisionBox *target);
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_COLLISIONBOXGROUP_H
