// Copyright (c) 2016 Benjamin Bäumler

#include "CollisionBoxGroup.h"
#include "../Entity.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace wyvern_engine {

CollisionBoxGroup::CollisionBoxGroup(std::string kind) : m_kind(kind) {}

CollisionBoxGroup::~CollisionBoxGroup() {}

void CollisionBoxGroup::AddCollisionBox(glm::vec3 a, glm::vec3 b) {
  m_children.push_back(
      std::unique_ptr<CollisionBox>(new CollisionBox(a, b, this)));
}

bool CollisionBoxGroup::CheckCollision(CollisionBoxGroup *targetGroup,
                                       std::string &kind) {
  auto trans = m_owner->GetComp<Transform>();
  auto matrix = glm::value_ptr(trans->GetMatrix());
  m_position = glm::vec3(matrix[12], matrix[13], matrix[14]);
  m_orientation = trans->GetTotalOrientation();

  matrix =
      glm::value_ptr(targetGroup->m_owner->GetComp<Transform>()->GetMatrix());
  targetGroup->m_position = glm::vec3(matrix[12], matrix[13], matrix[14]);
  ;

  for (auto &target : targetGroup->m_children) {
    if (CheckCollision(target.get(), kind)) {
      return true;
    }
  }
  return false;
}

bool CollisionBoxGroup::CheckCollision(CollisionBox *target,
                                       std::string &kind) {
  //        if(!m_owner->HasComponent<Transform>()) {
  //            return false;
  //        }
  for (auto &child : m_children) {
    if (child->CheckCollision(target)) {
      kind = m_kind;

      return true;
    }
  }
  return false;
}

CollisionBox::CollisionBox(glm::vec3 a, glm::vec3 b, CollisionBoxGroup *parent)
    : m_a(a.x, a.y, a.z), m_b(b.x, b.y, b.z), m_parent(parent) {}

CollisionBox::~CollisionBox() {}

//    QUE: start --- AABB Collision Detection --- 75%

bool CollisionBox::CheckCollision(CollisionBox *target) {
  auto a = m_parent->m_position + m_a;
  auto b = m_b;
  auto t_a = target->m_parent->m_position + target->m_a;
  auto t_b = target->m_b;

  b.x = std::fabs(b.x);
  b.y = std::fabs(b.y);
  b.z = std::fabs(b.z);

  t_b.x = std::fabs(t_b.x);
  t_b.y = std::fabs(t_b.y);
  t_b.z = std::fabs(t_b.z);

  bool x = std::fabs(a.x - t_a.x) <= (b.x + t_b.x);
  bool y = std::fabs(a.y - t_a.y) <= (b.y + t_b.y);
  bool z = std::fabs(a.z - t_a.z) <= (b.z + t_b.z);

  return x && y && z;
}

//    QUE: end --- AABB Collision Detection --- 75%

} // namespace wyvern_engine