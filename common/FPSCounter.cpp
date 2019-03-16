#include "pch.h"
#include "FPSCounter.h"
#include <chrono>

FPSCounter::FPSCounter()
{
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::increase()
{
	typedef std::chrono::high_resolution_clock TClock;

	static TClock::time_point start;

	if (0 == m_ticks)
	{
		start = TClock::now();
	}

	m_ticks++;

	TClock::time_point now = TClock::now();
	TClock::duration elapsed = now - start;

	std::chrono::milliseconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	if (1000 <= milliseconds)
	{
		m_fps = (double)m_ticks / (double)(milliseconds / 1000);
		start = now;
		m_ticks = 0;
	}
}

double FPSCounter::fps()
{
	return m_fps;
}
