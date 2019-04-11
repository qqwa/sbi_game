// Copyright (c) 2016 Benjamin Bäumler

#include "RenderingSystem.h"
#include "../World.h"
#include "../components/Camera.h"
#include "../components/DebugRenderer.h"
#include "../components/Material.h"
#include "../components/MeshRenderer.h"
#include "../components/Transform.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace wyvern_engine {
namespace rendering {

RenderingSystem::RenderingSystem(World *world, Entity *camera)
    : m_world(world), m_camera(camera) {}

RenderingSystem::~RenderingSystem() {}

void RenderingSystem::Render() {
  auto proj = m_camera->GetComp<Camera>()->GetProjection();
  auto view = m_camera->GetComp<Camera>()->GetTransform();

  auto shader = m_world->GetResourceManager()->LoadShader("res/shader/base");
  shader->Bind();
  glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE,
                     glm::value_ptr(proj));
  glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE,
                     glm::value_ptr(view));
  for (auto obj :
       m_world
           ->GetEntities<MeshRenderer, wyvern_engine::Material, Transform>()) {
    auto model = obj->GetComp<Transform>()->GetMatrix();
    auto material = obj->GetComp<wyvern_engine::Material>()->GetMaterial();

    glUniformMatrix4fv(shader->GetUniformLocation("model"), 1, GL_FALSE,
                       glm::value_ptr(model));
    glUniform4fv(shader->GetUniformLocation("materialColor"), 1,
                 glm::value_ptr(material->GetColor()));

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(shader->GetUniformLocation("texture_diffuse"), 0);
    glBindTexture(
        GL_TEXTURE_2D,
        material->GetTexture(Material::TextureType::DIFFUSE)->GetId());

    glActiveTexture(GL_TEXTURE0);

    obj->GetComp<MeshRenderer>()->Render();
  }

  // should be debug shader, but the shader got lost so we take the base shader
  // instead... shader =
  // m_world->GetResourceManager()->LoadShader("res/shader/base");
  // shader->Bind();
  // glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE,
  // glm::value_ptr(proj));
  // glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE,
  // glm::value_ptr(view));

  // for (auto obj : m_world->GetEntities<DebugRenderer,
  // wyvern_engine::Material, Transform>()) {
  //     auto model = obj->GetComp<Transform>()->GetMatrix();
  //     auto material = obj->GetComp<wyvern_engine::Material>()->GetMaterial();

  //     glUniformMatrix4fv(shader->GetUniformLocation("model"), 1, GL_FALSE,
  //     glm::value_ptr(model));
  //     glUniform4fv(shader->GetUniformLocation("materialColor"), 1,
  //     glm::value_ptr(material->GetColor()));

  //     obj->GetComp<DebugRenderer>()->Render();
  // }
}

} // namespace rendering
} // namespace wyvern_engine