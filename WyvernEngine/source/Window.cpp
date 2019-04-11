// Copyright (c) 2016 Benjamin Bäumler

#include "Window.h"

namespace wyvern_engine {

Window::Window(int width, int height, std::string title)
    : m_width(width), m_height(height) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_window);

  glfwSetWindowUserPointer(m_window, static_cast<void *>(this));

  glfwSetFramebufferSizeCallback(
      m_window, [](GLFWwindow *window, int width, int height) {
        auto owner = static_cast<Window *>(glfwGetWindowUserPointer(window));
        owner->SetSize(width, height);
        glViewport(0, 0, width, height);
      });
}

Window::Window(std::string title) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  m_window = glfwCreateWindow(mode->width, mode->height, title.c_str(),
                              glfwGetPrimaryMonitor(), nullptr);

  m_width = mode->width;
  m_height = mode->height;

  glfwMakeContextCurrent(m_window);

  glfwSetWindowUserPointer(m_window, static_cast<void *>(this));

  glfwSetFramebufferSizeCallback(
      m_window, [](GLFWwindow *window, int width, int height) {
        auto owner = static_cast<Window *>(glfwGetWindowUserPointer(window));
        owner->SetSize(width, height);
        glViewport(0, 0, width, height);
      });
}

Window::~Window() { glfwDestroyWindow(m_window); }

GLFWwindow *Window::GetGLFWWindow() { return m_window; }

bool Window::ShouldClose() const {
  return glfwWindowShouldClose(m_window) != 0;
}

void Window::SwapBuffers() {
  glfwSwapBuffers(m_window);
  m_deltaMouseX = 0.0f;
  m_deltaMouseY = 0.0f;
}

int Window::Height() { return m_height; }

int Window::Width() { return m_width; }

void Window::SetSize(const int &widht, const int &height) {
  m_width = widht;
  m_height = height;
}

void Window::GetMouseDelta(double &x, double &y) const {
  x = m_deltaMouseX;
  y = m_deltaMouseY;
}

void Window::SetMouseDelta(const double &x, const double &y) {
  m_deltaMouseX = x;
  m_deltaMouseY = y;
}

void Window::GetMouse(double &x, double &y) const {
  x = m_mouseX;
  y = m_mouseY;
}

void Window::SetMouse(const double &x, const double &y) {
  m_mouseX = x;
  m_mouseY = y;
}

} // namespace wyvern_engine