// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <ImGuiLib/ImGuiLib.hpp>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

#include <Glfw/CGuiGlfwGl3.hpp>
#include <Sdl2/CGuiSdl2Gl3.hpp>

#define GLFW_ELSE_SDL2

namespace dotname {

  ImGuiLib::ImGuiLib () {
    LOG_D_STREAM << libName_ << " ...constructed" << std::endl;
  }
  ImGuiLib::ImGuiLib (const std::filesystem::path& assetsPath) : ImGuiLib () {
    assetsPath_ = assetsPath;
    if (!assetsPath_.empty ()) {
      LOG_D_STREAM << "Assets path: " << assetsPath_ << std::endl;

#ifdef GLFW_ELSE_SDL2
      CGuiGlfwGl3 gui (assetsPath_);
#else
      CGuiSdl2Gl3 gui (assetsPath_);
#endif
      gui.runThread ();
      gui.joinThread ();

    } else {
      LOG_D_STREAM << "Assets path is empty" << std::endl;
    }
  }
  ImGuiLib::~ImGuiLib () {
    LOG_D_STREAM << libName_ << " ...destructed" << std::endl;
  }

} // namespace dotname