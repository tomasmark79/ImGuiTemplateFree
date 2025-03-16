#ifndef CGUIGLFWGL3_H
#define CGUIGLFWGL3_H

#include <string>
#include <thread>

class CGuiGlfwGl3 {
public:
  CGuiGlfwGl3(const std::string &assetsPath) : m_assetsPath(assetsPath) {};
  ~CGuiGlfwGl3() {};

  void Run();
  void runThread();
  void joinThread();

private:
  std::thread m_thread;
  std::string m_assetsPath;
};

void create_triangle(unsigned int &vbo, unsigned int &vao, unsigned int &ebo);
void render_conan_logo();

#endif