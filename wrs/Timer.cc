/*
 * Timer.cc
 *
 *  Created on: Jan 23, 2012
 *      Author: matthies
 */

#include "Timer.h"

Timer::Timer() {
	// TODO Auto-generated constructor stub

}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
}


double Timer::s2ms(double s) {
	return (s/10000.0);
}

timeval Timer::currentTime() {
	gettimeofday( &_time, NULL );
	return _time;
}

#ifdef __linux__


void Timer::print() {
	double sec, usec;
	sec = static_cast<double>( _time.tv_sec );
	usec = static_cast<double>( _time.tv_usec )/1E6;
	std::cout << " sec " << sec << " usec " << usec; // << std::endl;
}
#elif WIN32

void Timer::print() {
	//double sec = _time/CLOCKS_PER_SEC;
	std::cout << " sec " << _time.tv_sec << " usec " << _time.tv_usec; // << std::endl;
}
#endif
