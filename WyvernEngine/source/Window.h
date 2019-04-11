// Copyright (c) 2016 Benjamin Bäumler

#ifndef WYVERNENGINE_WINDOW_H
#define WYVERNENGINE_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace wyvern_engine {

/**
 * @brief Window is a wrapper for GLFWwindow
 */
class Window {
  GLFWwindow *m_window;
  int m_width;
  int m_height;
  double m_deltaMouseX;
  double m_deltaMouseY;
  double m_mouseX;
  double m_mouseY;

public:
  Window(int width, int height, std::string title);

  Window(std::string title);

  ~Window();

  /**
   * @brief GetGLFWWindow
   * @return The "real" GLFWwindow
   */
  GLFWwindow *GetGLFWWindow();

  /**
   * @brief Wrapper for glfwShouldWindowClose
   * @return Weather the window should close or not
   */
  bool ShouldClose() const;

  /**
   * @brief Wrapper for glfwSwapBuffers
   */
  void SwapBuffers();

  /**
   * @brief Height
   * @return The height of the Window
   */
  int Height();

  /**
   * @brief Width
   * @return The width of the window
   */
  int Width();

  /**
   * @brief SetSize is a Interface which can be used from callback functions
   * @param widht The new width of the window
   * @param height The new height of the window
   */
  void SetSize(const int &widht, const int &height);

  /**
   * @brief GetMouseDelta
   * @param x The delta of the x axis
   * @param y The delta of the y axis
   */
  void GetMouseDelta(double &x, double &y) const;

  /**
   * @brief SetMouseDelta is a Interface which can be used from callback
   * functions
   * @param x The new delta of the x axis
   * @param y The new delta of the y axis
   */
  void SetMouseDelta(const double &x, const double &y);

  /**
   * @brief GetMouse
   * @param x The x position of the mouse
   * @param y The y position of the mouse
   */
  void GetMouse(double &x, double &y) const;

  /**
   * @brief SetMouse is a Interface which can be used from callback functions
   * @param x The new x position of the mouse
   * @param y The new y position of the mouse
   */
  void SetMouse(const double &x, const double &y);
};

} // namespace wyvern_engine

#endif // WYVERNENGINE_WINDOW_H
