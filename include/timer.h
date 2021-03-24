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
    //! \brief Stores the current time as a means of staring the Timer. Must be called to restart Timer.
	//!
	void Start();

	//! \brief Get the elapsed time since the start of the timer in seconds.
	//!
	//! @return Elapsed time since start of timer in seconds.
	float GetElapsedTimeSeconds();

	//! \brief Get the elapsed time since the start of the timer in milliseconds.
	//!
	//! @return Elapsed time since start of timer in milliseconds.
	float GetElapsedTimeMilliSeconds();
};


#endif /* INCLUDE_TIMER_H_ */
