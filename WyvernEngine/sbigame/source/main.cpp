// Copyright (c) 2016 Benjamin Bäumler

#include "WorldManager.h"
#include "scripts/FreeMove.h"
#include <GL/glew.h>
#include <Profiler.h>
#include <ResourceManager.h>
#include <Window.h>
#include <World.h>
#include <chrono>
#include <components/Camera.h>
#include <iostream>

using namespace wyvern_engine;

int main() {
  glfwInit();
  Window *window = new Window("SBI");
  glewExperimental = static_cast<GLboolean>(true);
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return 1;
  }
  WorldManager manager(window);
  manager.Init();

  auto delta_timer = std::chrono::high_resolution_clock();
  auto last = delta_timer.now();

  while (!window->ShouldClose()) {
    auto cur = delta_timer.now();
    auto deltaTime =
        std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
            cur - last)
            .count();
    last = cur;
    manager.SetDeltaTime(deltaTime / 1000);

    glfwPollEvents();

    manager.Update();
    manager.Render();
    window->SwapBuffers();
  }

  glfwTerminate();
  return 0;
}