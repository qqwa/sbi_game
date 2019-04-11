// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_WORLDMANAGER_H
#define WYVERNENGINE_WORLDMANAGER_H

#include "../../source/World.h"

class WorldManager {
  wyvern_engine::World *m_activeWorld;
  bool m_updateWorld;
  wyvern_engine::World *m_menuWorld;
  std::shared_ptr<wyvern_engine::rendering::Texture> m_loadingTexture;
  bool m_paused;
  bool m_loading;
  bool m_showHighscore;
  GLuint m_vao;
  wyvern_engine::rendering::Shader::Ptr m_shader;
  float m_deltaTime;
  float m_fpsUpdateTimer;

  std::map<int, std::string, std::greater<int>> m_highscoreList;

  wyvern_engine::Window *m_window;
  wyvern_engine::Input *m_input;
  wyvern_engine::ResourceManager::Ptr m_resourceManager;

  static WorldManager *m_instance;

public:
  static std::string m_name;

  WorldManager(wyvern_engine::Window *window);

  ~WorldManager();

  static WorldManager *GetInstance();

  wyvern_engine::Window *GetWindow();

  wyvern_engine::World *GetMenu();

  void SetDeltaTime(float time);

  void Init();

  void Update();

  void Render();

  void LoadNewWorld(wyvern_engine::World *oldWorld, bool hardcore = false);

  void LoadNextWorld();

  void Pause();

  void Resume();

  void SetUpdateWorld(bool update);

  void ShowHighscore(int score);

  void CloseHighscore();

  std::map<int, std::string, std::greater<int>> GetHighscoreList();

  void AddScore(int score, std::string name);
};

#endif // WYVERNENGINE_WORLDMANAGER_H
