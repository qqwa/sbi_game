// Copyright (c) 2016 Benjamin Bäumler

#include "World.h"
#include "../sbigame/source/WorldManager.h"
#include "components/Camera.h"
#include <iostream>

namespace wyvern_engine {

World::World(ResourceManager::Ptr rm, Input *input, int width, int height)
    : m_rm(rm), m_input(input), m_isCamera(false) {
  m_screen = std::make_shared<nanogui::Screen>();
  input->AddScreen(m_screen.get());
  m_renderingSystem = nullptr;

  // create framebuffer
  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  int x = WorldManager::GetInstance()->GetWindow()->Width();
  int y = WorldManager::GetInstance()->GetWindow()->Height();
  // Generate texture
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Attach it to currently bound framebuffer object
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_texture, 0);

  GLuint rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
              << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

World::~World() {
  m_input->RemoveScreen(m_screen.get());

  m_entities.clear();
  m_scripts.clear();
  glDeleteTextures(1, &m_texture);
  glDeleteFramebuffers(1, &m_framebuffer);

  if (m_renderingSystem != nullptr) {
    delete m_renderingSystem;
  }
}

ResourceManager *World::GetResourceManager() { return m_rm.get(); }

Entity *World::CreateEntity() {
  auto entity = new Entity();
  m_entities.push_back(std::unique_ptr<Entity>(entity));

  // set entity values to default
  //    entity->m_id = ?
  entity->m_world = this;
  entity->m_parent = nullptr;

  return entity;
}

void World::RemoveEntity(const Entity *entity) {
  m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
                                  [&](std::unique_ptr<Entity> const &m_entity) {
                                    return m_entity.get() == entity;
                                  }),
                   m_entities.end());
}

std::vector<Entity *> World::GetEntities(std::string group) {
  std::vector<Entity *> entities;
  for (auto &entity : m_entities) {
    if (entity->GetGroup() == group) {
      entities.push_back(entity.get());
    }
  }
  return entities;
}

void World::RegisterScript(Script *script) {
  m_scripts.push_back(script);
  script->OnInit();
}

void World::RemoveScript(Script *script) {
  m_scripts.erase(std::remove(m_scripts.begin(), m_scripts.end(), script),
                  m_scripts.end());
}

void World::RunScripts() {
  for (const auto &script : m_scripts) {
    script->OnUpdate();
  }
}

GLuint World::Render() {
  if (m_renderingSystem == nullptr) {
    auto entites = GetEntities<Camera>();
    if (entites.size() != 0) {
      m_renderingSystem =
          new rendering::RenderingSystem(this, GetEntities<Camera>()[0]);
      m_isCamera = true;
    } else {
      m_renderingSystem = new rendering::RenderingSystem(this, nullptr);
    }
  }

  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  if (m_isCamera) {
    m_renderingSystem->Render();
  }

  m_screen->drawWidgets();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return m_texture;
}

void World::SetDeltaTime(float deltaTime) { m_detlaTime = deltaTime; }

float World::DeltaTime() { return m_detlaTime; }

nanogui::Screen *World::GetScreen() { return m_screen.get(); }

Input *World::GetInput() { return m_input; }

} // namespace wyvern_engine
