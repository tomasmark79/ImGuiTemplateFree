// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <ImGuiLib/ImGuiLib.hpp>
#include <ImGuiLib/version.h>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

#include <Glfw/CGuiGlfwGl3.hpp>
#include <Sdl2/CGuiSdl2Gl3.hpp>

#define GLFW_ELSE_SDL2

namespace library {

ImGuiLib::ImGuiLib(const std::string &assetsPath) : m_assetsPath(assetsPath) {
  LOG_IMPORTANT("ImGuiLib v." + std::string(IMGUILIB_VERSION) +
                " constructed.");
  LOG_DEBUG("Assets Path: " + this->m_assetsPath);

#ifdef GLFW_ELSE_SDL2
  CGuiGlfwGl3 gui(this->m_assetsPath);
#else
  CGuiSdl2Gl3 gui(this->m_assetsPath);
#endif
  gui.runThread();
  gui.joinThread();
}

ImGuiLib::~ImGuiLib() { LOG_DEBUG("ImGuiLib deconstructed."); }

} // namespace library