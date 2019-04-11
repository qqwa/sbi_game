// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_PLAYERCONTROLLER_H
#define WYVERNENGINE_PLAYERCONTROLLER_H

#include <EntityComponent.h>

class PlayerController : public wyvern_engine::Script {
  int m_energy;
  float m_energyTime;
  float m_time;
  float m_invincibleTimer;

public:
  int m_life;
  int m_level;
  int m_score;
  int m_difficulty;
  bool m_hardcore;

  PlayerController(int level, int score, int difficulty, float time_limit,
                   bool hardcore, int life);

  ~PlayerController();

  virtual void OnUpdate() override;

  void TakeDamage(int amount);

  void HealDamage(int amount);

  void TakeEnergy(int amount);

  void RestoreEnergy(int amount);

  void AddScore(int amount);

  void Lose();
};

#endif // WYVERNENGINE_PLAYERCONTROLLER_H
