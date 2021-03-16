/*
 * timer.h
 *
 *  Created on: Mar 16, 2021
 *      Author: cjgree13
 */
#include <chrono>

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

class Timer
{

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	std::chrono::duration<float> duration;

public:
	void Start();
	float GetElapsedTimeSeconds();
	float GetElapsedTimeMilliSeconds();
};


#endif /* INCLUDE_TIMER_H_ */
