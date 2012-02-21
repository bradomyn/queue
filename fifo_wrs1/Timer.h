/*
 * Timer.h
 *
 *  Created on: Jan 23, 2012
 *      Author: matthies
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>

#ifdef __linux__
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#elif WIN32
//#include <ctime>
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#endif

class Timer {
public:
	Timer();
	virtual ~Timer();

	struct timeval _time;
	timeval currentTime();

	void print();

	double s2ms(double s);

};

#endif /* TIMER_H_ */
