#ifndef UTILS_HPP
#define UTILS_HPP

#ifndef UTILS_ASSET_PATH
  #define UTILS_ASSET_PATH ""
#endif
#ifndef UTILS_FIRST_ASSET_FILE
  #define UTILS_FIRST_ASSET_FILE ""
#endif
#ifndef UTILS_ASSET_FILES_DIVIDED_BY_COMMAS
  #define UTILS_ASSET_FILES_DIVIDED_BY_COMMAS ""
#endif

#include <string>

// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

namespace FileSystemManager {
std::string read(const std::string &filename);
std::string getExecutableDirectory();
} // namespace FileSystemManager
#endif // UTILS_HPP