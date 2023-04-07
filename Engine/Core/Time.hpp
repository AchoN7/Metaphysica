#pragma once

#include <chrono>

using namespace std::chrono;

class Time {
public:
    Time() {
        s_startTime = system_clock::now();
        s_elapsedTime = milliseconds(0);
        s_lastTime = system_clock::now();
        s_isRunning = false;
    }

    void start() {
        s_startTime = system_clock::now();
        s_elapsedTime = milliseconds(0);
        s_isRunning = true;
    }

    void stop() {
        if (s_isRunning) {
            s_elapsedTime += duration_cast<milliseconds>(system_clock::now() - s_startTime);
            s_isRunning = false;
        }
    }

    double getElapsedTime() {
        return duration_cast<duration<double>>(s_elapsedTime).count();
    }

    double getDeltaTime() {
        auto now = system_clock::now();
        auto delta = now - s_lastTime;
        s_lastTime = now;
        return duration_cast<duration<double>>(delta).count();
    }

private:

    time_point<system_clock> s_startTime;
    milliseconds s_elapsedTime;
    time_point<system_clock> s_lastTime;
    bool s_isRunning;

};
