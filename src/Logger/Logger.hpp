#ifndef LOGGER_HPP
#define LOGGER_HPP

// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#ifdef _WIN32
  #ifndef NOMINMAX
    #define NOMINMAX
  // Disable min/max macros in windows.h to avoid conflicts with std::min/max
  // cxxopts.hpp uses std::min/max
  #endif

  // Undefine Raylib functions to avoid conflicts
  #define Rectangle WindowsRectangle
  #define CloseWindow WindowsCloseWindow
  #define ShowCursor WindowsShowCursor
  #define DrawText WindowsDrawText
  #define PlaySound WindowsPlaySound
  #define PlaySoundA WindowsPlaySoundA
  #define PlaySoundW WindowsPlaySoundW
  #define LoadImage WindowsLoadImage
  #define DrawTextEx WindowsDrawTextEx

  #include <windows.h>

  // Restore Raylib functions
  #undef Rectangle
  #undef CloseWindow
  #undef ShowCursor
  #undef DrawText
  #undef PlaySound
  #undef PlaySoundA
  #undef PlaySoundW
  #undef LoadImage
  #undef DrawTextEx

#endif

// Function name macros for different compilers
#if defined(__GNUC__) || defined(__clang__)
  #define FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
  #define FUNCTION_NAME __FUNCSIG__
#else
  #define FUNCTION_NAME __func__
#endif

// Optional compile-time log level control
#ifndef NDEBUG
  #define LOG_DEBUG_ENABLED
#endif

class Logger {
public:
  enum class Level {
    LOG_DEBUG,
    LOG_INFO,
    LOG_IMPORTANT,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
  };

  // C++11 Singleton
  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  void log(Level level, const std::string &message,
           const std::string &caller = "") {
    std::lock_guard<std::mutex> lock(logMutex);

    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);

    // Use thread-safe localtime
    std::tm now_tm;
#ifdef _WIN32
    localtime_s(&now_tm, &now_time);
#else
    localtime_r(&now_time, &now_tm);
#endif

    std::cout << "[" << std::put_time(&now_tm, "%d-%m-%Y %H:%M:%S") << "] ";

    // Only print caller if provided
    if (!caller.empty()) {
      std::cout << "[" << caller << "] ";
    }

    std::cout << "[" << levelToString(level) << "] ";

    setConsoleColor(level);
    std::cout << message;

    resetConsoleColor();
    std::cout << std::endl;

    // Also log to file if enabled
    if (m_logFile.is_open()) {
      m_logFile << "[" << std::put_time(&now_tm, "%d-%m-%Y %H:%M:%S") << "] ";
      if (!caller.empty()) {
        m_logFile << "[" << caller << "] ";
      }
      m_logFile << "[" << levelToString(level) << "] " << message << std::endl;
    }
  }

  Logger &operator<<(Level level) {
    m_currentLevel = level;
    return *this;
  }

  // Add setter for calling function
  Logger &setCallingFunction(const std::string &caller) {
    m_callingFunction = caller;
    return *this;
  }

  template <typename T> Logger &operator<<(const T &message) {
    std::lock_guard<std::mutex> lock(logMutex);
    m_messageStream << message;
    return *this;
  }

  Logger &operator<<(std::ostream &(*manip)(std::ostream &)) {
    if (manip == static_cast<std::ostream &(*)(std::ostream &)>(std::endl)) {
      log(m_currentLevel, m_messageStream.str(), m_callingFunction);
      // Reset state
      m_messageStream.str("");
      m_messageStream.clear();
      m_callingFunction = "";
    } else {
      m_messageStream << manip;
    }
    return *this;
  }

  void debug(const std::string &message, const std::string &caller = "") {
#ifdef LOG_DEBUG_ENABLED
    log(Level::LOG_DEBUG, message, caller);
#endif
  }

  void info(const std::string &message, const std::string &caller = "") {
    log(Level::LOG_INFO, message, caller);
  }

  void important(const std::string &message, const std::string &caller = "") {
    log(Level::LOG_IMPORTANT, message, caller);
  }

  void warning(const std::string &message, const std::string &caller = "") {
    log(Level::LOG_WARNING, message, caller);
  }

  void error(const std::string &message, const std::string &caller = "") {
    log(Level::LOG_ERROR, message, caller);
  }

  void critical(const std::string &message, const std::string &caller = "") {
    log(Level::LOG_CRITICAL, message, caller);
  }

  // Enable logging to file
  bool enableFileLogging(const std::string &filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    try {
      m_logFile.open(filename, std::ios::out | std::ios::app);
      return m_logFile.is_open();
    } catch (...) {
      return false;
    }
  }

  void disableFileLogging() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (m_logFile.is_open()) {
      m_logFile.close();
    }
  }

  // Set application name for logger
  void setApplicationName(const std::string &appName) {
    std::lock_guard<std::mutex> lock(logMutex);
    name = appName;
  }

private:
  std::string name =
      "DotNameLib"; // string name may be affected by SolutionRenamer.py
  std::mutex logMutex;
  std::string
      m_callingFunction; // Store calling function for stream style logging
  std::ofstream m_logFile;

  Level m_currentLevel = Level::LOG_INFO; // Default log level
  std::ostringstream m_messageStream;

  Logger() = default;
  ~Logger() {
    if (m_logFile.is_open()) {
      m_logFile.close();
    }
  }

  std::string levelToString(Level level) const {
    switch (level) {
    case Level::LOG_DEBUG:
      return "DEB";
    case Level::LOG_INFO:
      return "INF";
    case Level::LOG_IMPORTANT:
      return "IMP";
    case Level::LOG_WARNING:
      return "WAR";
    case Level::LOG_ERROR:
      return "ERR";
    case Level::LOG_CRITICAL:
      return "CRI";
    default:
      return "UNKNOWN";
    }
  }

  // Simplified and more maintainable color implementation
  void setConsoleColor(Level level) {
    struct ColorCode {
#ifdef _WIN32
      WORD windowsCode;
#endif
      const char *ansiCode;
    };

    static const std::map<Level, ColorCode> colorMap = {
        {Level::LOG_DEBUG,
         {
#ifdef _WIN32
             FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
#endif
             "\033[34m"}}, // Blue
        {Level::LOG_INFO,
         {
#ifdef _WIN32
             FOREGROUND_GREEN | FOREGROUND_INTENSITY,
#endif
             "\033[32m"}}, // Green
        {Level::LOG_IMPORTANT,
         {
#ifdef _WIN32
             FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
#endif
             "\033[35m"}}, // Magenta
        {Level::LOG_WARNING,
         {
#ifdef _WIN32
             FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
#endif
             "\033[33m"}}, // Yellow
        {Level::LOG_ERROR,
         {
#ifdef _WIN32
             FOREGROUND_RED | FOREGROUND_INTENSITY,
#endif
             "\033[31m"}}, // Red
        {Level::LOG_CRITICAL,
         {
#ifdef _WIN32
             FOREGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_BLUE,
#endif
             "\033[95m"}} // Purple
    };

    auto it = colorMap.find(level);
    if (it != colorMap.end()) {
#ifdef _WIN32
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              it->second.windowsCode);
#else
      std::cout << it->second.ansiCode;
#endif
    } else {
      resetConsoleColor();
    }
  }

  void resetConsoleColor() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_RED | FOREGROUND_GREEN |
                                FOREGROUND_BLUE);
#else
    std::cout << "\033[0m"; // Reset
#endif
  }
}; // class Logger

// Define LOG macro for easy access
#define LOG Logger::getInstance()

// Define macros for logging with caller information
#define LOG_DEBUG(msg) LOG.debug(msg, FUNCTION_NAME)
#define LOG_INFO(msg) LOG.info(msg, FUNCTION_NAME)
#define LOG_IMPORTANT(msg) LOG.important(msg, FUNCTION_NAME)
#define LOG_WARNING(msg) LOG.warning(msg, FUNCTION_NAME)
#define LOG_ERROR(msg) LOG.error(msg, FUNCTION_NAME)
#define LOG_CRITICAL(msg) LOG.critical(msg, FUNCTION_NAME)

// For stream style logging with caller info
#define LOG_WITH_CALLER LOG.setCallingFunction(FUNCTION_NAME)

// Define level-specific stream macros for consistent usage
#define LOG_D LOG_WITH_CALLER << Logger::Level::LOG_DEBUG
#define LOG_I LOG_WITH_CALLER << Logger::Level::LOG_INFO
#define LOG_M LOG_WITH_CALLER << Logger::Level::LOG_IMPORTANT
#define LOG_W LOG_WITH_CALLER << Logger::Level::LOG_WARNING
#define LOG_E LOG_WITH_CALLER << Logger::Level::LOG_ERROR
#define LOG_C LOG_WITH_CALLER << Logger::Level::LOG_CRITICAL

// Create a log usage section in the comments
/*
RECOMMENDED LOGGING USAGE:

1. Stream style (preferred):
   LOG_D << "Debug message" << value << std::endl;
   LOG_I << "Info message" << value << std::endl;
   LOG_W << "Warning message" << value << std::endl;
   LOG_E << "Error message" << value << std::endl;
   LOG_C << "Critical message" << value << std::endl;

2. Function style (when streaming isn't needed):
   LOG_DEBUG("Debug message");
   LOG_INFO("Info message");
   LOG_WARNING("Warning message");
   LOG_ERROR("Error message");
   LOG_CRITICAL("Critical message");
*/

#endif // LOGGER_HPP