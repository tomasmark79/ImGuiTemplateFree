// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include "Utils.hpp"
#include "Logger/Logger.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// CMAKE generating definitions
// ../cmake/tmplt-assets.cmake)

#ifdef _WIN32
  #include <windows.h>
#elif defined(__APPLE__)
  #include <limits.h>
  #include <mach-o/dyld.h>
#else // Linux
  #include <limits.h>
  #include <unistd.h>
#endif

namespace FileSystemManager {

std::string read(const std::string &filename) {
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::stringstream file_stream;
  try {
    file.open(filename.c_str());
    file_stream << file.rdbuf();
    file.close();
  } catch (const std::ifstream::failure &e) {
    LOG_ERROR("Error reading Shader File!");
  }
  return file_stream.str();
}

std::string getExecutableDirectory() {
  std::string path;

#ifdef _WIN32
  char buffer[MAX_PATH];
  GetModuleFileNameA(NULL, buffer, MAX_PATH);
  path = buffer;
  size_t pos = path.find_last_of("\\/");
  if (pos != std::string::npos) {
    path = path.substr(0, pos);
  }
#elif defined(__APPLE__)
  char buffer[PATH_MAX];
  uint32_t bufferSize = PATH_MAX;
  if (_NSGetExecutablePath(buffer, &bufferSize) == 0) {
    char realPath[PATH_MAX];
    if (realpath(buffer, realPath) != nullptr) {
      path = realPath;
      size_t pos = path.find_last_of("/");
      if (pos != std::string::npos) {
        path = path.substr(0, pos);
      }
    }
  }
#else
  char buffer[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (count != -1) {
    buffer[count] = '\0';
    path = buffer;
    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
      path = path.substr(0, pos);
    }
  }
#endif

  return path;
}

} // namespace FileSystemManager