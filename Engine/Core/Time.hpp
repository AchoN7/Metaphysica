#pragma once

#include <chrono>

using namespace std::chrono;

class Time {
public:
    Time() {
        m_startTime = system_clock::now();
        m_elapsedTime = milliseconds(0);
        m_lastTime = system_clock::now();
        m_isRunning = false;
    }

    void start() {
        m_startTime = system_clock::now();
        m_elapsedTime = milliseconds(0);
        m_isRunning = true;
    }

    void stop() {
        if (m_isRunning) {
            m_elapsedTime += duration_cast<milliseconds>(system_clock::now() - m_startTime);
            m_isRunning = false;
        }
    }

    double getElapsedTime() {
        return duration_cast<duration<double>>(m_elapsedTime).count();
    }

    double getDeltaTime() {
        auto now = system_clock::now();
        auto delta = now - m_lastTime;
        m_lastTime = now;
        return duration_cast<duration<double>>(delta).count();
    }

private:

    time_point<system_clock> m_startTime;
    milliseconds m_elapsedTime;
    time_point<system_clock> m_lastTime;
    bool m_isRunning;

};
