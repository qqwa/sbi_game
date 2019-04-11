// Copyright (c) 2016 Benjamin Bäumler

#include "WorldManager.h"
#include "UI.h"
#include "components/SegmentRef.h"
#include "level/Level.h"
#include "scripts/FreeMove.h"
#include "scripts/PlayerController.h"
#include <components/Camera.h>
#include <components/CollisionBoxGroup.h>
#include <components/Material.h>
#include <components/Transform.h>
#include <iostream>

using namespace wyvern_engine;

void CollisionDetection(World *world);

WorldManager *WorldManager::m_instance = nullptr;
std::string WorldManager::m_name = "";

WorldManager::WorldManager(Window *window)
    : m_paused(false), m_loading(true), m_showHighscore(false),
      m_fpsUpdateTimer(0.f) {
  m_instance = this;
  m_resourceManager = std::make_shared<ResourceManager>();
  m_window = window;
  m_activeWorld = nullptr;
  m_menuWorld = nullptr;

  m_loadingTexture =
      m_resourceManager->LoadTexture("res/textures/loadingScreen.jpg");
  m_shader = m_resourceManager->LoadShader("res/shader/target");

  // create renderplane
  GLfloat screenVerticies[] = {-1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                               0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                               -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                               1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

  GLuint screenVbo;
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &screenVbo);
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, screenVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenVerticies), &screenVerticies,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)(2 * sizeof(GLfloat)));
  glBindVertexArray(0);

  glfwSwapInterval(1);
  glClearColor(0.3, 0.2, 0.2, 1.0);

  Render();
  window->SwapBuffers();
}

WorldManager::~WorldManager() {
  if (m_activeWorld != nullptr) {
    delete m_activeWorld;
  }
  if (m_menuWorld != nullptr) {
    delete m_menuWorld;
  }
}

WorldManager *WorldManager::GetInstance() { return m_instance; }

Window *WorldManager::GetWindow() { return m_window; }

wyvern_engine::World *WorldManager::GetMenu() { return m_menuWorld; }

void WorldManager::SetDeltaTime(float time) {
  m_deltaTime = time;
  m_fpsUpdateTimer += time;
  // check for fps timer on screen
  if (0.5f < m_fpsUpdateTimer) {
    m_fpsUpdateTimer = 0.0f;
    if (m_activeWorld != nullptr) {
      for (auto widget : m_activeWorld->GetScreen()->children()) {
        if (widget->id() == "fps") {
          auto fps_label = dynamic_cast<nanogui::Label *>(widget->childAt(0));
          int fps = static_cast<int>(1 / time);
          fps_label->setCaption("Fps: " + std::to_string(fps));
        }
      }
    }
  }
}

void WorldManager::Init() {
  m_input = new Input(m_window);

  m_menuWorld = new World(m_resourceManager, m_input, m_window->Width(),
                          m_window->Height());
  CreateMenu(*m_menuWorld);

  m_loading = false;
}

bool gen = false;

void WorldManager::Update() {
  if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_ESCAPE)) {
    if (!gen) {
      gen = true;

      if (m_showHighscore) {
        CloseHighscore();
        return;
      }

      if (m_activeWorld == nullptr) {
        glfwSetWindowShouldClose(GetWindow()->GetGLFWWindow(), true);
        return;
      }

      if (m_updateWorld) {
        if (m_paused) {
          Resume();
        } else {
          Pause();
        }
      }
    }
  } else {
    gen = false;
  }

  if (m_activeWorld != nullptr && !m_paused && m_updateWorld) {
    m_activeWorld->SetDeltaTime(m_deltaTime);
    m_activeWorld->RunScripts();
    CollisionDetection(m_activeWorld);
  }
}

void WorldManager::Render() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glEnable(GL_STENCIL);
  glCullFace(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if (m_loading) {
    // render loading screen to texture
    m_shader->Bind();
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, m_loadingTexture->GetId());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    return;
  }

  if (m_activeWorld == nullptr) {
    // render menu with just new game option
    auto texture = m_menuWorld->Render();

    m_shader->Bind();
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    return;
  }
  // render active world to texture
  auto world_texture = m_activeWorld->Render();

  if (m_paused) {
    // render menu with active_world in background
    auto texture = m_menuWorld->Render();

    m_shader->Bind();
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

  } else {
    // render texture to screen
    m_shader->Bind();
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, world_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }
}

void WorldManager::LoadNewWorld(World *oldWorld, bool hardcore) {
  m_loading = true;
  Render();
  m_window->SwapBuffers();

  if (m_showHighscore) {
    CloseHighscore();
  }

  if (oldWorld == nullptr) {
    if (m_activeWorld != nullptr) {
      delete m_activeWorld;
    }

    m_activeWorld = new World{m_resourceManager, m_input, m_window->Width(),
                              m_window->Height()};
    fin::GenLevel(*m_activeWorld, 1, 0, hardcore, 100);

  } else {
    auto playerController = m_activeWorld->GetEntities<PlayerController>()[0]
                                ->GetComp<PlayerController>();

    auto newWorld = new World{m_resourceManager, m_input, m_window->Width(),
                              m_window->Height()};
    if (playerController->m_hardcore) {
      fin::GenLevel(*newWorld, playerController->m_level + 1,
                    playerController->m_score, playerController->m_hardcore,
                    playerController->m_life);
    } else {
      fin::GenLevel(*newWorld, playerController->m_level + 1,
                    playerController->m_score, playerController->m_hardcore,
                    100);
    }

    delete m_activeWorld;

    m_activeWorld = newWorld;
  }

  m_resourceManager->UploadMeshData();
  m_resourceManager->ClearMeshData();
  m_resourceManager->Clear();

  m_loading = false;
  m_updateWorld = true;
  m_paused = false;
  Resume();
}

void WorldManager::LoadNextWorld() { LoadNewWorld(m_activeWorld); }

void WorldManager::Pause() {
  m_paused = true;
  auto screen = m_menuWorld->GetScreen();
  auto windowWidget = screen->childAt(0);
  auto window = windowWidget->window();

  for (auto child : windowWidget->children()) {
    if (dynamic_cast<nanogui::Button *>(child)->caption() == "Resume") {
      if (m_activeWorld == nullptr) {
        child->setVisible(false);
      } else {
        child->setVisible(true);
      }
    } else {
      child->setVisible(true);
    }
  }

  window->center();
  screen->performLayout();

  glfwSetInputMode(m_window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void WorldManager::Resume() {
  m_paused = false;
  auto screen = m_menuWorld->GetScreen();
  auto windowWidget = screen->childAt(0);
  auto window = windowWidget->window();

  for (auto child : windowWidget->children()) {
    child->setVisible(false);
  }

  window->center();
  screen->performLayout();

  if (m_updateWorld) {
    glfwSetInputMode(m_window->GetGLFWWindow(), GLFW_CURSOR,
                     GLFW_CURSOR_DISABLED);
  }
}

void WorldManager::SetUpdateWorld(bool update) { m_updateWorld = update; }

void WorldManager::ShowHighscore(int score) {

  if (m_showHighscore) {
    return;
  }

  if (!m_paused) {
    delete m_activeWorld;
    m_activeWorld = nullptr;
    Pause();
    m_paused = false;
  }

  CreateHighscore(m_menuWorld, score);

  m_showHighscore = true;
}

void WorldManager::CloseHighscore() {
  m_showHighscore = false;

  auto screen = m_menuWorld->GetScreen();

  for (auto widget : screen->children()) {
    if (widget->id() == "highscore") {
      screen->removeChild(widget);
    }
  }
}

std::map<int, std::string, std::greater<int>> WorldManager::GetHighscoreList() {
  return m_highscoreList;
}

void WorldManager::AddScore(int score, std::string name) {
  m_highscoreList.insert({score, name});
}

void CollisionDetection(World *world) {
  auto player = world->GetEntities("player")[0];
  auto playerTransform = player->GetComp<Transform>();
  auto playerCbg =
      player->GetEntities<CollisionBoxGroup>()[0]->GetComp<CollisionBoxGroup>();
  auto playerController = player->GetComp<PlayerController>();
  auto tunnel = world->GetEntities("tunnel")[0];
  auto segRef = tunnel->GetComp<SegmentRef>();

  auto segments = segRef->GetSegments(playerTransform->GetPosition().z);

  std::string kind;
  std::vector<Entity *> to_remove;

  for (auto segment : segments) {

    // check collision with tunnel
    if (segment->GetComp<CollisionBoxGroup>()->CheckCollision(playerCbg,
                                                              kind) &&
        kind == "tunnel") {

      playerController->TakeDamage(25);
      playerController->TakeEnergy(25);
      auto player = world->GetEntities("player")[0];
      auto trans = player->GetComp<Transform>();
      trans->SetPosition({0, 0, trans->GetPosition().z});
      trans->SetOrientation(glm::quat());
      auto freemove = player->GetComp<FreeMove>();
      if (freemove->speed > 1) {
        freemove->speed = 1;
      }
    }

    for (auto entity : segment->GetEntities<CollisionBoxGroup>()) {
      // check collision with other entities
      if (entity->GetComp<CollisionBoxGroup>()->CheckCollision(playerCbg,
                                                               kind)) {
        if (kind == "o1") {
          playerController->TakeDamage(10);
        } else if (kind == "o2") {
          playerController->TakeDamage(25);
        } else if (kind == "h1") {
          playerController->RestoreEnergy(10);
          playerController->AddScore(100);
          to_remove.push_back(entity);
        } else if (kind == "h2") {
          playerController->RestoreEnergy(25);
          playerController->AddScore(250);
          to_remove.push_back(entity);
        } else if (kind == "c") {
          playerController->AddScore(50);
          to_remove.push_back(entity);
        } else if (kind == "goal") {
          auto worldManager = WorldManager::GetInstance();
          glfwSetInputMode(worldManager->GetWindow()->GetGLFWWindow(),
                           GLFW_CURSOR, GLFW_CURSOR_NORMAL);
          playerController->AddScore(playerController->m_difficulty);
          worldManager->SetUpdateWorld(false);
          CreateWinScreen(*world);
        } else if (kind == "lose") {
          playerController->Lose();
        }
      }
    }
  }
  for (auto entity : to_remove) {
    world->RemoveEntity(entity);
  }
}