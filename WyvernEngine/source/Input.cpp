// Copyright (c) 2016 Benjamin Bäumler

#include "Input.h"
#include <iostream>

namespace wyvern_engine {

std::vector<nanogui::Screen *> Input::screens;

Input::Input(Window *window) : m_window(window) {
  auto win = m_window->GetGLFWWindow();

  glfwSetCursorPosCallback(
      win, [](GLFWwindow *window, double xpos, double ypos) {
        auto owner = static_cast<Window *>(glfwGetWindowUserPointer(window));
        double oldX, oldY;
        owner->GetMouse(oldX, oldY);
        owner->SetMouse(xpos, ypos);
        owner->SetMouseDelta(xpos - oldX, oldY - ypos);
        // nanogui
        for (auto screen : Input::screens) {
          screen->cursorPosCallbackEvent(xpos, ypos);
        }
      });

  glfwSetMouseButtonCallback(
      win, [](GLFWwindow *, int button, int action, int modifiers) {
        for (auto screen : Input::screens) {
          screen->mouseButtonCallbackEvent(button, action, modifiers);
        }
      });

  glfwSetKeyCallback(
      win, [](GLFWwindow *, int key, int scancode, int action, int mods) {
        for (auto screen : Input::screens) {
          screen->keyCallbackEvent(key, scancode, action, mods);
        }
      });

  glfwSetCharCallback(win, [](GLFWwindow *, unsigned int codepoint) {
    for (auto screen : Input::screens) {
      screen->charCallbackEvent(codepoint);
    }
  });

  glfwSetDropCallback(win, [](GLFWwindow *, int count, const char **filenames) {
    for (auto screen : Input::screens) {
      screen->dropCallbackEvent(count, filenames);
    }
  });

  glfwSetScrollCallback(win, [](GLFWwindow *, double x, double y) {
    for (auto screen : Input::screens) {
      screen->scrollCallbackEvent(x, y);
    }
  });

  glfwSetFramebufferSizeCallback(win, [](GLFWwindow *, int width, int height) {
    for (auto screen : Input::screens) {
      screen->resizeCallbackEvent(width, height);
    }
  });
}

Input::~Input() {}

void Input::AddScreen(nanogui::Screen *screen) {
  screen->initialize(m_window->GetGLFWWindow(), false);
  Input::screens.push_back(screen);
}

void Input::RemoveScreen(nanogui::Screen *screen) {
  Input::screens.erase(std::remove(screens.begin(), screens.end(), screen));
}

bool Input::GetAction(Input::Action action) {
  switch (action) {
  case Action::SHOT:
    if (glfwJoystickPresent(0)) {
      int count;
      auto button = glfwGetJoystickButtons(0, &count);
      return button[0];
    }
    return glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_SPACE) != 0;
  case Action::SHIFT:
    if (glfwJoystickPresent(0)) {
      int count;
      auto button = glfwGetJoystickButtons(0, &count);
      return button[1];
    }
    return glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_LEFT_SHIFT) != 0;
  }
}

float Input::GetAxis(Input::Axis axis) {
  switch (axis) {
  case Axis::HORIZONTAL:
    if (glfwJoystickPresent(0)) {
      int count;
      auto axis = glfwGetJoystickAxes(0, &count);
      return axis[0];
    }

    if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_A)) {
      return -1.0f;
    } else if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_D)) {
      return 1.0f;
    } else {
      return 0.0f;
    }
  case Axis::VERTICAL:
    if (glfwJoystickPresent(0)) {
      int count;
      auto axis = glfwGetJoystickAxes(0, &count);
      return axis[1];
    }
    if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_S)) {
      return -1.0f;
    } else if (glfwGetKey(m_window->GetGLFWWindow(), GLFW_KEY_W)) {
      return 1.0f;
    } else {
      return 0.0f;
    }
  case Axis::M_HORIZONTAL:
    double x, y;
    m_window->GetMouseDelta(x, y);
    return static_cast<float>(x);
  case Axis::M_VERTICAL:
    double x1, y1;
    m_window->GetMouseDelta(x1, y1);
    return static_cast<float>(y1);
  }
}

}; // namespace wyvern_engine