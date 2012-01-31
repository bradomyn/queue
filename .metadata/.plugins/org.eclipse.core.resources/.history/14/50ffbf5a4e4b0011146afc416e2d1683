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
#endif

class Timer {
public:
	Timer();
	virtual ~Timer();

	timeval currentTime();

	void print();

	struct timeval _time;

	double s2ms(double s);

};

#endif /* TIMER_H_ */
