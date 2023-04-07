#pragma once

#include <sstream>
#include <string>
#include <queue>
#include <mutex>
#include <list>

enum class LogType {
    ERROR,
    DEBUG,
    INFO,
    OPENGL
};

class Logger {
public:
    
    template<typename... Args>
    static void log(LogType type, const Args&... args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream oss;
        (oss << ... << args);
        s_logEntries.emplace_back(std::tuple<LogType, std::string>(type, oss.str()));
    }

    static auto getMessages() {
        std::lock_guard<std::mutex> lock(s_mutex);
        return s_logEntries;
    }

    static bool hasMessages() {
        std::lock_guard<std::mutex> lock(s_mutex);
        return !s_logEntries.empty();
    }

private:
    static inline std::list<std::tuple<LogType, std::string>> s_logEntries;
    static inline std::mutex s_mutex;
};

//static, gets logs from all sources
//prints them to the gui console