#ifndef CGUIGLFWGL3_H
#define CGUIGLFWGL3_H

#include <string>
#include <thread>
#include <filesystem>

class CGuiGlfwGl3 {
public:
  CGuiGlfwGl3 (const std::string& assetsPath) : assetsPath_ (assetsPath) {};
  ~CGuiGlfwGl3 () {};

  void Run ();
  void runThread ();
  void joinThread ();

private:
  std::thread thread_;
  std::filesystem::path assetsPath_;
};

void create_triangle (unsigned int& vbo, unsigned int& vao, unsigned int& ebo);
void render_conan_logo ();

#endif