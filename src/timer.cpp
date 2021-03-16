/*
 * timer.cpp
 *
 *  Created on: Mar 16, 2021
 *      Author: cjgree13
 */


#include "timer.h"

void Timer::Start()
{
	// store time at start of timer
	start = std::chrono::high_resolution_clock::now();
}

float Timer::GetElapsedTimeSeconds()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	return duration.count();
}

float Timer::GetElapsedTimeMilliSeconds()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	float ms = duration.count() * 1000.0f;
	return ms;
}
