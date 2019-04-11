// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_LEVEL_H
#define WYVERNENGINE_LEVEL_H

#include <World.h>

void GenLevel(wyvern_engine::World &world, glm::vec3 color);

void GenLevel(wyvern_engine::World &world, int section_count);

void Plattform(wyvern_engine::World &world);

namespace ex {
void GenLevel(wyvern_engine::World &world, int section_count);
}

namespace fin {
void GenLevel(wyvern_engine::World &world, int level, int score, bool hardcore,
              int life);
}

#endif // WYVERNENGINE_LEVEL_H
