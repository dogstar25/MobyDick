#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock
{

public:

	static Clock& instance();
	void init();
	void update();
	void calcFps();
	bool hasMetGameLoopSpeed();
	std::chrono::duration<float> timeElapsed();

	int fps() {
		return m_fps;
	}

private:
	Clock();
	~Clock();

	void resetGameLoopTimeAccum();

	int m_current_frame_cnt;
	int m_fps;
	std::chrono::steady_clock::time_point m_current_time_snapshot;
	std::chrono::duration<double> m_fps_time_accum;
	std::chrono::duration<double> m_gameloop_time_accum;
	std::chrono::duration<double> m_milisecsPerUpdate;

};

#endif