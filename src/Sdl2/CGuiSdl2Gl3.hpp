#ifndef CGUISDL2GL3_H
#define CGUISDL2GL3_H

#include <thread>

class CGuiSdl2Gl3 {
public:
  CGuiSdl2Gl3() {};
  ~CGuiSdl2Gl3() {};

  void Run();
  void runThread();
  void joinThread();

private:
  std::thread m_thread;
};

#endif