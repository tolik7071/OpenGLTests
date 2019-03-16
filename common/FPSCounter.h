#pragma once

typedef unsigned long long TTicks;

class FPSCounter
{
public:
	FPSCounter();
	~FPSCounter();

public:
	void increase();
	double fps();

private:
	TTicks m_ticks;
	double m_fps;
};
