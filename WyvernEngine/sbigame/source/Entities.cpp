// Copyright (c) 2016 Benjamin Bäumler

#include "Entities.h"
#include "scripts/FreeMove.h"
#include "scripts/HammerScript.h"
#include "scripts/PlayerController.h"
#include "scripts/SwingScript.h"
#include <components/Camera.h>
#include <components/CollisionBoxGroup.h>
#include <components/Material.h>
#include <components/MeshRenderer.h>
#include <components/Transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <rendering/Material.h>

using namespace wyvern_engine;

class Rotator : public Script {
  float m_speed;

public:
  Rotator(float speed) : m_speed(speed) {}
  virtual void OnUpdate() override {
    m_owner->GetComp<Transform>()->Rotate(m_speed * m_world->DeltaTime(),
                                          {0, 1, 0});
  }
};

void CreatePlayer(wyvern_engine::World &world, int level, int score,
                  int difficulty, float time_limit, bool hardcore, int life) {
  auto root = world.CreateEntity();
  root->SetGroup("player");
  auto body = world.CreateEntity();
  auto camera = world.CreateEntity();
  root->AddEntity(body);
  root->AddEntity(camera);

  auto shader = world.GetResourceManager()->LoadShader("res/shader/base");
  auto mat = std::make_shared<rendering::Material>(shader);
  auto mesh = world.GetResourceManager()->LoadMesh("res/models/player.obj");
  auto meshScythe =
      world.GetResourceManager()->LoadMesh("res/models/scythe.obj");
  auto texture =
      world.GetResourceManager()->LoadTexture("res/textures/scythe.jpg");
  mat->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
  mat->SetTexture(rendering::Material::TextureType::DIFFUSE, texture);

  root->Assign(new Transform(glm::vec3{0.0f, 0.0f, 0.0f}));
  root->Assign(new FreeMove(1.0, 45.0));
  root->Assign(new PlayerController(level, score, difficulty, time_limit,
                                    hardcore, life));

  auto cbg = new CollisionBoxGroup("player");
  cbg->AddCollisionBox(glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0.1));

  body->Assign(cbg);
  body->Assign(new Transform(glm::vec3{0.0f, 0.0f, 0.0f}, glm::quat(),
                             glm::vec3{0.1, 0.1, 0.1}));
  body->Assign(new MeshRenderer(mesh));
  body->Assign(new Material(mat));

  auto scytheAnchor = root->CreateEntity();
  scytheAnchor->Assign(new Transform(glm::vec3(0, 0, 0)));
  scytheAnchor->Assign(new Rotator(45));

  auto scythe = scytheAnchor->CreateEntity();
  scythe->Assign(new Transform(glm::vec3(0.3, 0, 0), glm::quat(),
                               glm::vec3(0.075, 0.075, 0.075)));
  scythe->Assign(new Rotator(30));
  scythe->Assign(new MeshRenderer(meshScythe));
  scythe->Assign(new Material(mat));

  camera->Assign(new Camera(glm::perspective(45.0f, 1.0f, 0.01f, 1000.0f)));
  camera->Assign(new Transform(glm::vec3{0.0f, 0.3f, 1.0f}));
}

void Pillar(wyvern_engine::World *world, wyvern_engine::Entity *segment,
            glm::vec3 pos) {
  auto obstacle = segment->CreateEntity();

  auto shader = world->GetResourceManager()->LoadShader("res/shader/base");
  auto mat = std::make_shared<rendering::Material>(shader);
  auto texture =
      world->GetResourceManager()->LoadTexture("res/textures/pillar.jpg");
  auto mesh = world->GetResourceManager()->LoadMesh("res/models/pillar.obj");
  mat->SetTexture(rendering::Material::TextureType::DIFFUSE, texture);
  auto trans = new Transform(pos);
  auto cbg = new CollisionBoxGroup("o1");
  cbg->AddCollisionBox(glm::vec3(0, 0, 0), glm::vec3(3, 0.49, 0.49));

  obstacle->Assign(cbg);
  obstacle->Assign(trans);
  obstacle->Assign(new MeshRenderer(mesh));
  obstacle->Assign(new Material(mat));
}

void Swing(wyvern_engine::World *world, wyvern_engine::Entity *segment,
           glm::vec3 pos, bool direction) {
  auto obstacle = segment->CreateEntity();

  auto shader = world->GetResourceManager()->LoadShader("res/shader/base");
  auto mat = std::make_shared<rendering::Material>(shader);
  auto texture =
      world->GetResourceManager()->LoadTexture("res/textures/scythe.jpg");
  auto mesh = world->GetResourceManager()->LoadMesh("res/models/swing.obj");
  mat->SetTexture(rendering::Material::TextureType::DIFFUSE, texture);
  auto trans = new Transform(pos);
  auto cbg = new CollisionBoxGroup("o2");

  // wood
  cbg->AddCollisionBox(glm::vec3(0, 0, 0), glm::vec3(0.12, 2, 0.4));
  // blades
  cbg->AddCollisionBox(glm::vec3(0, 1.45, 0), glm::vec3(0.8, 0.20, 0.25));
  cbg->AddCollisionBox(glm::vec3(0, -0.05, 0), glm::vec3(0.8, 0.20, 0.25));
  cbg->AddCollisionBox(glm::vec3(0, -1.45, 0), glm::vec3(0.8, 0.20, 0.25));

  obstacle->Assign(cbg);
  obstacle->Assign(trans);
  obstacle->Assign(new MeshRenderer(mesh));
  obstacle->Assign(new Material(mat));
  obstacle->Assign(new SwingScript(0.5, direction));
}

void Goal(wyvern_engine::World *world, wyvern_engine::Entity *segment) {
  auto goal = segment->CreateEntity();

  auto shader = world->GetResourceManager()->LoadShader("res/shader/base");
  auto mat = std::make_shared<rendering::Material>(shader);
  auto texture =
      world->GetResourceManager()->LoadTexture("res/textures/goal.jpg");
  auto mesh = world->GetResourceManager()->LoadMesh("res/models/goal.obj");
  mat->SetColor(glm::vec4(0, 0, 0, 0));
  mat->SetTexture(rendering::Material::TextureType::DIFFUSE, texture);

  goal->Assign(
      new Transform(glm::vec3{0, 0, 0}, glm::quat(), glm::vec3(0.8, 0.8, 0.8)));
  auto goalCbg = new CollisionBoxGroup("goal");
  goalCbg->AddCollisionBox({0, 0, 0}, {30, 30, 2});
  goal->Assign(goalCbg);
  goal->Assign(new MeshRenderer(mesh));
  goal->Assign(new Material(mat));
}

void Lose(wyvern_engine::World *world, wyvern_engine::Entity *segment) {
  auto lose = segment->CreateEntity();

  lose->Assign(new Transform(glm::vec3{0, 0, 0}));
  auto goalCbg = new CollisionBoxGroup("lose");
  goalCbg->AddCollisionBox({0, 0, 0}, {30, 30, 2});
  lose->Assign(goalCbg);
}

void Hammer(wyvern_engine::World *world, wyvern_engine::Entity *segment,
            glm::vec3 pos, bool imp) {
  auto hammer = segment->CreateEntity();

  auto shader = world->GetResourceManager()->LoadShader("res/shader/base");
  auto mat = std::make_shared<rendering::Material>(shader);
  auto texture =
      world->GetResourceManager()->LoadTexture("res/textures/scythe.jpg");
  auto mesh = world->GetResourceManager()->LoadMesh("res/models/hammer.obj");
  mat->SetTexture(rendering::Material::TextureType::DIFFUSE, texture);
  auto trans = new Transform(pos);

  trans->SetScale({0.5, 0.5, 0.5});

  CollisionBoxGroup *cbg;
  if (imp) {
    mat->SetColor(glm::vec3(0.05, 0, 0));
    cbg = new CollisionBoxGroup("h2");
  } else {
    mat->SetColor(glm::vec3(0, 0, 0.02));
    cbg = new CollisionBoxGroup("h1");
  }
  cbg->AddCollisionBox(glm::vec3(0, 0, 0), glm::vec3(0.27, 0.27, 0.27));

  hammer->Assign(cbg);
  hammer->Assign(trans);
  hammer->Assign(new HammerScript());
  hammer->Assign(new MeshRenderer(mesh));
  hammer->Assign(new Material(mat));
}

void Coin(wyvern_engine::World *world, wyvern_engine::Entity *segment,
          glm::vec3 pos) {
  auto coin = segment->CreateEntity();

  auto shader = world->GetResourceManager()->LoadShader("res/shader/base");
  auto mat = std::make_shared<rendering::Material>(shader);
  auto texture =
      world->GetResourceManager()->LoadTexture("res/textures/coin.jpg");
  auto mesh = world->GetResourceManager()->LoadMesh("res/models/coin.obj");
  mat->SetTexture(rendering::Material::TextureType::DIFFUSE, texture);
  auto trans = new Transform(pos);

  CollisionBoxGroup *cbg;
  cbg = new CollisionBoxGroup("c");
  cbg->AddCollisionBox(glm::vec3(0, 0, 0), glm::vec3(0.25, 0.25, 0.25));

  coin->Assign(cbg);
  coin->Assign(trans);
  coin->Assign(new HammerScript());
  coin->Assign(new MeshRenderer(mesh));
  coin->Assign(new Material(mat));
}
