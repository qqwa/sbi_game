// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_INPUT_H
#define WYVERNENGINE_INPUT_H

#include "Window.h"
#include <memory>
#include <nanogui/nanogui.h>

namespace wyvern_engine {

/**
 * @brief Input manages the Keyboard and the Mouse.
 *
 *
 */
class Input {
  Window *m_window;
  float m_x;
  float m_y;

public:
  static std::vector<nanogui::Screen *> screens;
  enum class Action { SHOT, SHIFT };
  enum class Axis { HORIZONTAL, VERTICAL, M_HORIZONTAL, M_VERTICAL };

  Input(Window *window);

  ~Input();

  /**
   * @brief AddScreen adds a screen to the callback functions
   * @param screen The screen to add
   */
  void AddScreen(nanogui::Screen *screen);

  /**
   * @brief RemoveScreen removes a screen from the callback functions
   * @param screen The screen to remove
   */
  void RemoveScreen(nanogui::Screen *screen);

  /**
   * @brief GetAction is wrapper function to receive weather a key is pressed or
   * not
   * @param action The action to check, a action can possible be mapped to
   * multiple keys
   * @return true if any key mapped to the action is pressed
   */
  bool GetAction(Input::Action action);

  /**
   * @brief GetAxis combines two keys or actions to an axis
   * @param axis
   * @return The current value of the axis, it ranges between -1.0 and 1.0
   */
  float GetAxis(Input::Axis axis);
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_INPUT_H
