// Copyright (c) 2016 Benjamin Bäumler

#include "UI.h"
#include "WorldManager.h"
#include "scripts/PlayerController.h"

using namespace nanogui;

nanogui::Widget *scoreWidget(ref<Window> window, int score);

nanogui::Widget *nextLvlButtons(ref<Window> window, int score);

nanogui::Widget *top10(ref<Window> window,
                       std::map<int, std::string, std::greater<int>>, int,
                       bool *);

nanogui::Widget *scoreEntry(Widget *, int, int, std::string);

void CreateFpsLabel(wyvern_engine::World &world) {
  // init widget
  nanogui::Widget *widget = new nanogui::Widget(world.GetScreen());
  widget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
                                           nanogui::Alignment::Minimum, 0, 20));
  widget->setPosition({10, 10});
  widget->setId("fps");

  // add components
  nanogui::Label *label = new nanogui::Label(widget, "Fps:\t20", "sans", 24);

  world.GetScreen()->performLayout();
}

void CreateLevelInfo(wyvern_engine::World &world) {
  // init widget
  nanogui::Widget *widget = new nanogui::Widget(world.GetScreen());
  widget->setId("levelinfo");
  widget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
                                           nanogui::Alignment::Maximum, 0, 20));
  widget->setPosition({10, 200});

  // add components
  nanogui::Label *score = new nanogui::Label(widget, "Score:\t20", "sans", 24);
  nanogui::Label *time = new nanogui::Label(widget, "Time:\t20", "sans", 24);

  world.GetScreen()->performLayout();
}

void CreatePlayerInfo(wyvern_engine::World &world) {
  //    init widget
  Widget *widget = new nanogui::Widget(world.GetScreen());
  widget->setId("playerInfo");
  widget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
                                           nanogui::Alignment::Maximum, 0, 20));
  widget->setPosition({10, 100});

  // add components
  nanogui::Label *life = new nanogui::Label(widget, "Life:\t20", "sans", 24);
  nanogui::Label *energy =
      new nanogui::Label(widget, "Energy:\t20", "sans", 24);

  //    life->setColor({1, 0, 0, 1});
  //    energy->setColor({1, 1, 0, 1});

  world.GetScreen()->performLayout();
}

void CreateMenu(wyvern_engine::World &world) {
  FormHelper *gui = new FormHelper(world.GetScreen());
  ref<Window> nanoguiWindow =
      gui->addWindow(Eigen::Vector2i(0, 0), "Stone to Brain Interface");
  auto resume =
      gui->addButton("Resume", []() { WorldManager::GetInstance()->Resume(); });
  gui->addButton("New Game",
                 []() { WorldManager::GetInstance()->LoadNewWorld(nullptr); });
  gui->addButton("New Hardcore Game", []() {
    WorldManager::GetInstance()->LoadNewWorld(nullptr, true);
  });
  gui->addButton("Highscore",
                 []() { WorldManager::GetInstance()->ShowHighscore(-1); });
  gui->addButton("Exit Game", []() {
    glfwSetWindowShouldClose(
        WorldManager::GetInstance()->GetWindow()->GetGLFWWindow(), true);
  });

  resume->setVisible(false);

  nanoguiWindow->center();
  //    nanoguiWindow->setModal(true);
  world.GetScreen()->performLayout();
}

void CreateWinScreen(wyvern_engine::World &world) {
  auto playerController =
      world.GetEntities<PlayerController>()[0]->GetComp<PlayerController>();
  std::string levelName = "Level " + std::to_string(playerController->m_level);
  int score = playerController->m_score;
  ;

  FormHelper *gui = new FormHelper(world.GetScreen());
  ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(0, 0), levelName);
  gui->addGroup("YOU WON!");
  gui->addGroup("Score");
  gui->addWidget("", scoreWidget(nanoguiWindow, score));
  gui->addGroup("Continue");
  gui->addWidget("", nextLvlButtons(nanoguiWindow, score));

  nanoguiWindow->center();
  world.GetScreen()->performLayout();
}

void CreateLoseScreen(wyvern_engine::World &world) {
  auto playerController =
      world.GetEntities<PlayerController>()[0]->GetComp<PlayerController>();
  std::string levelName = "Level " + std::to_string(playerController->m_level);
  int score = playerController->m_score;
  ;

  FormHelper *gui = new FormHelper(world.GetScreen());
  ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(0, 0), levelName);
  gui->addGroup("YOU LOST :(");
  gui->addGroup("Score");
  gui->addWidget("", scoreWidget(nanoguiWindow, score));
  gui->addGroup("End");

  auto widget = new Widget(nanoguiWindow);
  widget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
                                           nanogui::Alignment::Minimum, 0, 20));
  Button *end = new Button(widget, "End Game");
  end->setCallback([score]() {
    // go to highscore list and add entry
    WorldManager::GetInstance()->ShowHighscore(score);
  });

  gui->addWidget("", widget);

  nanoguiWindow->center();
  world.GetScreen()->performLayout();
}

void CreateHighscore(wyvern_engine::World *world, int score) {
  auto screen = world->GetScreen();

  std::map<int, std::string, std::greater<int>> highscoreList =
      WorldManager::GetInstance()->GetHighscoreList();

  FormHelper *gui = new FormHelper(screen);
  ref<Window> nanoguiWindow =
      gui->addWindow(Eigen::Vector2i(0, 0), "Highscore");
  nanoguiWindow->setId("highscore");
  gui->addGroup("Top 10");
  bool newScore;
  gui->addWidget("", top10(nanoguiWindow, highscoreList, score, &newScore));

  if (score != -1 && newScore) {
    auto textArea = gui->addVariable("Your Name", WorldManager::m_name);
    textArea->setFixedWidth(75);
    gui->addButton("Submit Score", [score]() {
      WorldManager::GetInstance()->AddScore(score, WorldManager::m_name);
      WorldManager::GetInstance()->CloseHighscore();
    });
  }

  auto endButton = gui->addButton(
      "Close", []() { WorldManager::GetInstance()->CloseHighscore(); });

  nanoguiWindow->center();
  world->GetScreen()->performLayout();
}

nanogui::Widget *scoreWidget(ref<Window> window, int score) {
  auto widget = new nanogui::Widget(window);
  widget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
                                           nanogui::Alignment::Minimum, 0, 20));
  nanogui::Label *label = new nanogui::Label(
      widget, "Credits earned:\t" + std::to_string(score), "sans", 24);
  return widget;
}

nanogui::Widget *nextLvlButtons(ref<Window> window, int score) {
  auto widget = new nanogui::Widget(window);
  widget->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                           nanogui::Alignment::Minimum, 0, 20));
  nanogui::Button *button1 = new Button(widget, "End Game");
  nanogui::Button *button2 = new Button(widget, "Next Level");

  button1->setCallback([score]() {
    // go to highscore list and add entry
    WorldManager::GetInstance()->ShowHighscore(score);
  });

  button2->setCallback([]() { WorldManager::GetInstance()->LoadNextWorld(); });

  return widget;
}

nanogui::Widget *top10(ref<Window> window,
                       std::map<int, std::string, std::greater<int>> map,
                       int score, bool *newScore) {
  auto res = new Widget(window);
  res->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
                                        nanogui::Alignment::Minimum, 0, 5));

  bool highscore = false;
  *newScore = false;
  bool match = false;
  int i = 0;
  for (auto entry : map) {
    i++;
    if (score != -1 && !highscore) {
      // check if we made it into the top10
      if (entry.first < score) {
        scoreEntry(res, i, score, "Your Score");
        highscore = true;
        *newScore = true;
        i++;
      }
      if (entry.first == score) {
        match = true;
      }
    }
    scoreEntry(res, i, entry.first, entry.second);
  }
  i++;
  if (score != -1 && !highscore && !match) {
    scoreEntry(res, i, score, "Your Score");
    *newScore = true;
  }

  return res;
}

nanogui::Widget *scoreEntry(Widget *widget, int place, int score,
                            std::string name) {
  auto res = new Widget(widget);
  res->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                        nanogui::Alignment::Minimum, 0, 20));

  auto label = new Label(res, std::to_string(place) + ".");
  label = new Label(res, name);
  label = new Label(res, std::to_string(score) + " Points");
}