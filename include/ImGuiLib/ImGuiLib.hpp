#ifndef __IMGUILIB_H__
#define __IMGUILIB_H__

// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <string>
// Public API

namespace library {

class ImGuiLib {
public:
  ImGuiLib(const std::string &assetsPath);
  ~ImGuiLib();

  // alternatively, you can use a getter function
  const std::string getAssetsPath() const { return m_assetsPath; }

private:
  std::string m_assetsPath;
};

} // namespace library

#endif // __IMGUILIB_H__
