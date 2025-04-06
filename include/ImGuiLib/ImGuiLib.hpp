// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#ifndef __IMGUILIB_HPP
#define __IMGUILIB_HPP

#include <ImGuiLib/version.h>
#include <filesystem>
#include <string>

// Public API

namespace dotname {

  class ImGuiLib {

    const std::string libName_ = std::string ("ImGuiLib v.") + IMGUILIB_VERSION;
    std::filesystem::path assetsPath_;

  public:
    ImGuiLib ();
    ImGuiLib (const std::filesystem::path& assetsPath);
    ~ImGuiLib ();

    const std::filesystem::path getAssetsPath () const {
      return assetsPath_;
    }
    void setAssetsPath (const std::filesystem::path& assetsPath) {
      assetsPath_ = assetsPath;
    }
  };

} // namespace dotname

#endif // __IMGUILIB_HPP