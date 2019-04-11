// Copyright (c) 2016 Benjamin Bäumler

#include "PlayerController.h"
#include "../UI.h"
#include "../WorldManager.h"
#include "FreeMove.h"
#include <components/Material.h>

PlayerController::PlayerController(int level, int score, int difficulty,
                                   float time_limit, bool hardcore, int life)
    : m_life(life), m_energy(100), m_energyTime(0), m_level(level),
      m_score(score), m_difficulty(difficulty), m_time(time_limit),
      m_hardcore(hardcore) {}

PlayerController::~PlayerController() {}

void PlayerController::OnUpdate() {
  m_energyTime += m_world->DeltaTime();
  m_time -= m_world->DeltaTime();

  auto tmp = m_invincibleTimer;

  if (m_invincibleTimer >= .0) {
    m_invincibleTimer -= m_world->DeltaTime();
  }

  if (tmp > .0 && m_invincibleTimer <= .0) {
    auto mat = m_owner->GetEntities<wyvern_engine::Material>()[0]
                   ->GetComp<wyvern_engine::Material>();
    mat->GetMaterial()->SetColor(glm::vec4(0, 0, 0, 1));
  }

  if (m_energyTime >= .4) {
    m_energyTime -= .4;
    m_energy -= 1;
  }
  // check if player is alive
  if (m_life < 1 || m_energy < 1 || m_time < 0) {
    // game over
    auto worldManager = WorldManager::GetInstance();
    glfwSetInputMode(worldManager->GetWindow()->GetGLFWWindow(), GLFW_CURSOR,
                     GLFW_CURSOR_NORMAL);
    worldManager->SetUpdateWorld(false);
    CreateLoseScreen(*m_world);
  }

  auto screen = m_world->GetScreen();

  for (auto child : screen->children()) {
    if (child->id() == "playerInfo") {
      auto life = dynamic_cast<nanogui::Label *>(child->childAt(0));
      auto energy = dynamic_cast<nanogui::Label *>(child->childAt(1));

      life->setCaption("Life: " + std::to_string(m_life));
      energy->setCaption("Energy: " + std::to_string(m_energy));
    }
    if (child->id() == "levelinfo") {
      //            Score:	20 Score\t20 then time
      auto score = dynamic_cast<nanogui::Label *>(child->childAt(0));
      auto time = dynamic_cast<nanogui::Label *>(child->childAt(1));

      score->setCaption("Score:\t" + std::to_string(m_score));
      time->setCaption("Time:\t" + std::to_string(m_time));
    }
  }
}

void PlayerController::TakeDamage(int amount) {
  if (m_invincibleTimer <= 0.0) {
    m_life -= amount;
    m_invincibleTimer = 0.75;
    auto mat = m_owner->GetEntities<wyvern_engine::Material>()[0]
                   ->GetComp<wyvern_engine::Material>();
    mat->GetMaterial()->SetColor(glm::vec4(0, 0, 0, 0.3));

    auto player = m_world->GetEntities("player")[0];
    auto freemove = player->GetComp<FreeMove>();
    freemove->speed -= 0.2 * amount;
    if (freemove->speed < 0.5) {
      freemove->speed = 0.5;
    }
  }
}

void PlayerController::HealDamage(int amount) { m_life += amount; }

void PlayerController::TakeEnergy(int amount) {
  if (m_invincibleTimer <= 0.0) {
    m_energy -= amount;
    m_invincibleTimer = 0.75;
    auto mat = m_owner->GetEntities<wyvern_engine::Material>()[0]
                   ->GetComp<wyvern_engine::Material>();
    mat->GetMaterial()->SetColor(glm::vec4(0, 0, 0, 0.3));
  }
}

void PlayerController::RestoreEnergy(int amount) { m_energy += amount; }

void PlayerController::AddScore(int amount) {
  m_score += amount * m_level;
  if (m_hardcore) {
    m_score += amount * m_level;
  }
}

void PlayerController::Lose() {
  m_life = 0;
  m_energy = 0;
}
