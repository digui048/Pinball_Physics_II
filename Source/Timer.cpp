// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"

#include "raylib.h"

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	started_at = GetTime();
}

double Timer::ReadSec() const
{
	return (GetTime() - started_at);
}

void TimerBumper::Start()
{
	started_at = GetTime();
}

void TimerBumper::StopTime()
{
	//Stop the time
	started_at = 0;
}

double TimerBumper::ReadSec() const
{
	return (GetTime() - started_at);
}

bool TimerBumper::IsStopped() const
{
	return started_at == 0;
}

TimerBumper::TimerBumper()
{
	Start();
}