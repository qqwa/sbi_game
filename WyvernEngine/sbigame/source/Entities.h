// Copyright (c) 2016 Benjamin Bäumler

#ifndef SBI_GAME_ENTITIES_H
#define SBI_GAME_ENTITIES_H

#include <World.h>

void CreatePlayer(wyvern_engine::World &world, int level, int score,
                  int difficulty, float time_limit, bool hardcore, int life);

void Pillar(wyvern_engine::World *world, wyvern_engine::Entity *segment,
            glm::vec3 pos);

void Swing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
           glm::vec3 pos, bool direction);

void Goal(wyvern_engine::World *world, wyvern_engine::Entity *segment);

void Lose(wyvern_engine::World *world, wyvern_engine::Entity *segment);

void Hammer(wyvern_engine::World *world, wyvern_engine::Entity *segment,
            glm::vec3 pos, bool imp);

void Coin(wyvern_engine::World *world, wyvern_engine::Entity *segment,
          glm::vec3 pos);

#endif // SBI_GAME_ENTITIES_H
