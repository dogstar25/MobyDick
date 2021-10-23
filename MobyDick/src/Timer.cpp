#include "Timer.h"
#include "DebugPanel.h"
#include "DynamicTextManager.h"
#include "Util.h"
#include <iostream>

Timer::Timer(float targetDuration) : m_targetDuration(targetDuration) 
{

    if (targetDuration == 0.) {
        m_infiniteLifetime = true;
    }
}

std::chrono::duration<float> Timer::timeElapsed() 
{ 
    std::chrono::duration<float> timeDiff = std::chrono::steady_clock::now() - m_timeSnapshot;
    return timeDiff;
}

float Timer::percentTargetMet()
{
    float percentTargetMet{};
    if (m_infiniteLifetime == false) {
        percentTargetMet = timeRemaining() / m_targetDuration;
    }
    return percentTargetMet;
}

std::chrono::duration<float> Timer::timeRemaining()
{
    auto timeRemaining =  m_targetDuration - timeElapsed();
    return timeRemaining;
}

bool Timer::hasMetTargetDuration()
{
    if (m_infiniteLifetime == false) {
        std::chrono::steady_clock::time_point nowSnapshot = std::chrono::steady_clock::now();

        std::chrono::duration<float> timeDiff = timeElapsed();

        if (timeDiff >= m_targetDuration) {

            //Accumulate the timedif to be used for other calculations if needed
            m_accumulatedTimeDiff += timeDiff;
            //reset the timer
            m_timeSnapshot = nowSnapshot;
            return true;
        }
    }

    return false;
}

void Timer::reset()
{
    m_timeSnapshot = std::chrono::steady_clock::now();
    //m_accumulatedTimeDiff = std::chrono::duration < float>(0);

}

float Timer::calculateFPS(int& frameCount)
{
    float fps{};
    if (frameCount > 100) {
        fps = frameCount / m_accumulatedTimeDiff.count();
        frameCount = 0;
        m_accumulatedTimeDiff = std::chrono::duration < float>(0);
    }

    return fps;
}
