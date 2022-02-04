#include "Clock.h"


using namespace std::chrono_literals;

Clock::Clock::Clock()
{

}

Clock::Clock::~Clock()
{

}


Clock& Clock::instance()
{
	static Clock singletonInstance;
	return singletonInstance;
}

void Clock::init()
{
	m_current_frame_cnt = 0;
	m_fps_time_accum = 0ns;
	m_current_time_snapshot = std::chrono::steady_clock::now();
	//m_milisecsPerUpdate =	std::chrono::milliseconds(GameConfig::instance().gameLoopStep());

}
void Clock::update()
{
	std::chrono::steady_clock::time_point now_time;
	std::chrono::duration<double> time_diff;

	now_time = std::chrono::steady_clock::now();
	time_diff = now_time - m_current_time_snapshot;
	m_current_time_snapshot = now_time;

	m_fps_time_accum += time_diff;
	m_gameloop_time_accum += time_diff;
}

void Clock::calcFps()
{
	m_current_frame_cnt++;

	//Every 100 frames calculate how much time ellapsed, to calculate the FPS average
	if (m_current_frame_cnt >= 100)
	{
		m_fps = (int)(m_current_frame_cnt / m_fps_time_accum.count());
		m_fps_time_accum = 0ns;
		m_current_frame_cnt = 0;
	}

	resetGameLoopTimeAccum();

}

void Clock::resetGameLoopTimeAccum()
{
	m_gameloop_time_accum = 0ns;
}

bool Clock::hasMetGameLoopSpeed()
{
	if (m_gameloop_time_accum >= m_milisecsPerUpdate)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::chrono::duration<float> Clock::timeElapsed()
{
	return m_gameloop_time_accum;
}
