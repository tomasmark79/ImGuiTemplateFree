#ifndef CGUISDL2GL3_H
#define CGUISDL2GL3_H

#include <string>
#include <thread>

class CGuiSdl2Gl3 {
public:
  CGuiSdl2Gl3(const std::string &assetsPath) : m_assetsPath(assetsPath) {};
  ~CGuiSdl2Gl3() {};

  void Run();
  void runThread();
  void joinThread();

private:
  std::thread m_thread;
  std::string m_assetsPath;
};

#endif