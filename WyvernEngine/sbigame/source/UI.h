// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_UI_H
#define WYVERNENGINE_UI_H

#include <World.h>
#include <nanogui/nanogui.h>

void CreateFpsLabel(wyvern_engine::World &world);

void CreateLevelInfo(wyvern_engine::World &world);

void CreatePlayerInfo(wyvern_engine::World &world);

void CreateMenu(wyvern_engine::World &world);

void CreateWinScreen(wyvern_engine::World &world);

void CreateLoseScreen(wyvern_engine::World &world);

void CreateHighscore(wyvern_engine::World *world, int score);

#endif // WYVERNENGINE_UI_H
