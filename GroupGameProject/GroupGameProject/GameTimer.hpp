#pragma once
#pragma once

#include <chrono>

using Clock = std::chrono::high_resolution_clock;

class GameTimer
{
public:

    GameTimer() {
        Reset();
    }

    void Reset() {
        m_start = Clock::now();
    }
    int ElapsedMinutes() const;
    double ElapsedSeconds() const;
    long long ElapsedMilliseconds() const;
private:
    std::chrono::time_point<Clock> m_start;
};

