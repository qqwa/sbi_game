// Copyright (c) 2016 Benjamin Bäumler

#include "Level.h"
#include "../Entities.h"
#include "../UI.h"
#include "../components/SegmentRef.h"
#include <components/CollisionBoxGroup.h>
#include <components/DebugRenderer.h>
#include <components/Material.h>
#include <components/MeshRenderer.h>
#include <components/Transform.h>
#include <fstream>
#include <iomanip>
#include <memory>
#include <random>
#include <rendering/DebugRender.h>

using namespace wyvern_engine;

namespace ex {

void GenSeg(wyvern_engine::World &world, Entity *root, Transform transform,
            int pos, rendering::Material::Ptr material,
            rendering::Mesh::Ptr mesh, CollisionBoxGroup *cbg) {
  auto segment = root->CreateEntity();

  float length = 1.f;
  float diameter = 4.f;

  transform.Translate(transform.GetForward() * (length / 2));

  switch (pos) {
  case 0: // top

    transform.Translate(transform.GetUp() * (diameter / 2));
    transform.Rotate(90, glm::vec3(1, 0, 0));
    transform.Rotate(90, glm::vec3(0, 0, 1));

    cbg->AddCollisionBox(glm::vec3(0, 1, 0) * (diameter / 2) +
                             glm::vec3(0, 0, -1) * (length / 2),
                         glm::vec3((diameter / 2), 0.05, length * 1.5));
    break;
  case 1: // right
    transform.Translate(transform.GetRight() * (diameter / 2));
    transform.Rotate(270, glm::vec3(0, 1, 0));
    cbg->AddCollisionBox(glm::vec3(1, 0, 0) * (diameter / 2) +
                             glm::vec3(0, 0, -1) * (length / 2),
                         glm::vec3(0.05, (diameter / 2), length * 1.5));
    break;
  case 2: // botom
    transform.Translate(-transform.GetUp() * (diameter / 2));
    transform.Rotate(-90, glm::vec3(1, 0, 0));
    transform.Rotate(90, glm::vec3(0, 0, 1));

    cbg->AddCollisionBox(glm::vec3(0, -1, 0) * (diameter / 2) +
                             glm::vec3(0, 0, -1) * (length / 2),
                         glm::vec3((diameter / 2), 0.05, length * 1.5));
    break;
  case 3: // left
    transform.Translate(-transform.GetRight() * (diameter / 2));
    transform.Rotate(90, glm::vec3(0, 1, 0));
    cbg->AddCollisionBox(glm::vec3(-1, 0, 0) * (diameter / 2) +
                             glm::vec3(0, 0, -1) * (length / 2),
                         glm::vec3(0.05, (diameter / 2), length * 1.5));
    break;
  }

  auto trans = new Transform(transform);
  segment->Assign(trans);
  segment->Assign(new MeshRenderer(mesh));
  segment->Assign(new Material(material));
}

} // namespace ex

namespace fin {
std::vector<Entity *> GenTunnel(wyvern_engine::World *world, int length);

void SimplePillar(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                  glm::vec3 pos);

void SimpleSwing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                 glm::vec3 pos, bool direction);

void PillarPillar(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                  glm::vec3 pos, glm::vec3 pos2);

void PillarSwing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                 glm::vec3 pos, glm::vec3 pos2, bool direction);

void SwingSwing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                glm::vec3 pos, glm::vec3 pos2);

void GenLevel(wyvern_engine::World &world, int level, int score, bool hardcore,
              int life) {
  auto length = 20 + level * level * 2;
  auto time_limit = length * std::pow(0.95, level);
  auto obstacle_count = length * 0.3 * std::pow(1.05, level);
  auto diffculty = obstacle_count * 50 *
                   std::pow(1.4, level); // == score for finishing level
  auto coin_count = length / 3;
  auto hammer_count = length / 15;

  CreatePlayer(world, level, score, diffculty, time_limit, hardcore, life);
  CreateFpsLabel(world);
  CreateLevelInfo(world);
  CreatePlayerInfo(world);

  auto tunnelSegments = GenTunnel(&world, length);

  // lose and goal lose
  Lose(&world, tunnelSegments[1]);
  Goal(&world, tunnelSegments[length + 7]);

  std::random_device rd;
  auto mt = std::mt19937(rd());
  auto distSeg = std::uniform_int_distribution<int>(10, 5 + length);
  auto distPos = std::uniform_real_distribution<float>(-1.6f, 1.6f);
  auto distObstacle =
      std::uniform_int_distribution<int>(0, (diffculty / length) * 10 + 500);

  std::vector<int> selected(obstacle_count);
  int curDifficulty = diffculty;

  for (int i = 0; i < obstacle_count && curDifficulty > 0; i++) {

    // select segment
    int seg = distSeg(mt);
    while (std::find(selected.begin(), selected.end(), seg) != selected.end()) {
      seg = distSeg(mt);
    }
    selected.push_back(seg);

    // gen random pos
    auto pos1 = glm::vec3(distPos(mt), distPos(mt), distPos(mt));
    auto pos2 = glm::vec3(distPos(mt), distPos(mt), distPos(mt));

    bool placed = false;
    // chose obstacle to place
    while (!placed) {
      int obstacleValue =
          distObstacle(mt) - static_cast<int>(1 / ((1.f / level * level)));
      if (obstacleValue <= 50) { // pillar
        curDifficulty -= 50;
        placed = true;
        SimplePillar(&world, tunnelSegments[seg], pos1);
      } else if (obstacleValue <= 350) { // swing
        if (curDifficulty - 350 >= 0) {
          curDifficulty -= 350;
          placed = true;
          SimpleSwing(&world, tunnelSegments[seg], pos1, distSeg(mt) % 2 == 0);
        }
      } else if (obstacleValue <= 550) { // pillarswing
        if (curDifficulty - 550 >= 0) {
          curDifficulty -= 550;
          placed = true;
          PillarSwing(&world, tunnelSegments[seg], pos1, pos2,
                      distSeg(mt) % 2 == 0);
        }
      } else if (obstacleValue <= 1450) { // pillarpillar
        if (curDifficulty - 1450 >= 0) {
          curDifficulty -= 1450;
          placed = true;
          PillarPillar(&world, tunnelSegments[seg], pos1, pos2);
        }
      } else if (obstacleValue <= 4000) { // swing swing
        if (curDifficulty - 4000 >= 0) {
          curDifficulty -= 4000;
          placed = true;
          SwingSwing(&world, tunnelSegments[seg], pos1, pos2);
        }
      }
    }
  }

  // place coins
  auto distCoin = std::uniform_real_distribution<float>(-0.9f, 0.9f);
  auto coinPos = glm::vec3(distPos(mt), distPos(mt), 0);

  for (int i = 0; i <= 5 + length; i += 2) {
    Coin(&world, tunnelSegments[i], coinPos);

    auto tmpPos = coinPos;
    tmpPos += glm::vec3(distCoin(mt), distCoin(mt), 0);
    while (!(-1.5 < tmpPos.x && tmpPos.x < 1.5) ||
           !(-1.5 < tmpPos.y && tmpPos.y < 1.5)) {
      tmpPos = coinPos;
      tmpPos += glm::vec3(distCoin(mt), distCoin(mt), 0);
    }
    coinPos = tmpPos;
  }

  // place hammer

  for (int i = 0; i < length / 10; i++) {
    // select segment
    int seg = distSeg(mt);
    while (std::find(selected.begin(), selected.end(), seg) != selected.end()) {
      seg = distSeg(mt);
    }
    selected.push_back(seg);
    auto pos = glm::vec3(distPos(mt), distPos(mt), -0.5);
    Hammer(&world, tunnelSegments[seg], pos, distSeg(mt) % 2 == 0);
  }
}

void SimplePillar(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                  glm::vec3 pos) {
  pos.x = 0;
  pos.z = 0;
  Pillar(world, segment, pos);
}

void SimpleSwing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                 glm::vec3 pos, bool direction) {
  pos.y = 0;
  pos.z = 0;
  Swing(world, segment, pos, direction);
}

void PillarPillar(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                  glm::vec3 pos, glm::vec3 pos2) {
  pos.x = 0;
  pos.z = 0;
  pos2.x = 0;
  pos2.z = 0;
  if (pos.y - pos2.y < 0.3 && pos.y - pos2.y > -0.3) {
    if (pos.y + 0.7 < 1.8) {
      pos.y += 0.7;
    } else {
      pos.y -= 0.7;
    }
  }

  Pillar(world, segment, pos);
  Pillar(world, segment, pos2);
}

void PillarSwing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                 glm::vec3 pos, glm::vec3 pos2, bool direction) {
  pos.x = 0;
  pos.z = 0.5;
  pos2.y = 0;
  pos2.z = -0.5f;
  Pillar(world, segment, pos);
  Swing(world, segment, pos2, direction);
}

void SwingSwing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
                glm::vec3 pos, glm::vec3 pos2) {
  pos.y = 0;
  pos.z = 0;
  pos2.y = 0;
  pos2.z = 0;
  Swing(world, segment, pos, false);
  Swing(world, segment, pos2, true);
}

std::vector<Entity *> GenTunnel(wyvern_engine::World *world, int length) {
  auto root = world->CreateEntity();
  root->SetGroup("tunnel");
  std::vector<Entity *> segments;

  auto mesh = world->GetResourceManager()->LoadMesh("res/models/wall.obj");
  auto shader = world->GetResourceManager()->LoadShader("res/shader/base");
  auto texture =
      world->GetResourceManager()->LoadTexture("res/textures/wall.jpg");
  auto mat1 = std::make_shared<rendering::Material>(shader);
  mat1->SetTexture(rendering::Material::TextureType::DIFFUSE, texture);

  Transform currentPosition(glm::vec3(0, 0, 5));
  auto forward = glm::vec3(0, 0, -1);

  for (int i = 0; i < length + 55; i++) {
    auto curSeg = root->CreateEntity();
    curSeg->Assign(new Transform(currentPosition));
    segments.push_back(curSeg);

    auto cbg = new CollisionBoxGroup("tunnel");
    curSeg->Assign(cbg);

    ex::GenSeg(*world, curSeg, currentPosition, 0, mat1, mesh, cbg);
    ex::GenSeg(*world, curSeg, currentPosition, 1, mat1, mesh, cbg);
    ex::GenSeg(*world, curSeg, currentPosition, 2, mat1, mesh, cbg);
    ex::GenSeg(*world, curSeg, currentPosition, 3, mat1, mesh, cbg);

    currentPosition.Translate(forward);
  }

  root->Assign(new SegmentRef(segments, 5, currentPosition.GetPosition().z, 1));
  return segments;
}

} // namespace fin