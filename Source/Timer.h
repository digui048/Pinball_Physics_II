#pragma once

#include "Globals.h"
#include "Timer.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	double ReadSec() const;

private:

    // Start time in seconds
	double started_at;   
};

class TimerBumper
{
public:

	// Constructor
	TimerBumper();

	void Start();
	void StopTime();
	double ReadSec() const;

	bool IsStopped() const;

private:
	// Start time in seconds
	double started_at;
};