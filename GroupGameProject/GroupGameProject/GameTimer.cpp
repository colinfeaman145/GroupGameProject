#include "GameTimer.hpp"

int GameTimer::ElapsedMinutes() const {
    return std::chrono::duration_cast<std::chrono::minutes>(Clock::now() - m_start).count();
}

double GameTimer::ElapsedSeconds() const {
    return std::chrono::duration<double>(Clock::now() - m_start).count();
}

long long GameTimer::ElapsedMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_start).count();
}
