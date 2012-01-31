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

#ifdef __linux__
timeval Timer::currentTime() {
	//struct timeval tp;
	//double sec, usec, start, end;
	gettimeofday( &_time, NULL );
	//sec = static_cast<double>( tp.tv_sec );
	//usec = static_cast<double>( tp.tv_usec )/1E6;
	//start = sec + usec;
	//end = sec + usec;
	// Time calculation (in seconds)
	//double time = end - start;
	return _time;
}

void Timer::print() {
	double sec, usec;
	sec = static_cast<double>( _time.tv_sec );
	usec = static_cast<double>( _time.tv_usec )/1E6;
	std::cout << " sec " << sec << " usec " << usec; // << std::endl;
}
#elif WIN32
struct timeval Timer::currentTime() {
	struct _timeb timebuffer;
	_ftime_s( &timebuffer );
	t->tv_sec = (long)timebuffer.time;
	t->tv_usec = 1000*timebuffer.millitm;
	//_time = clock();
	return _time;
}

void Timer::print() {
	//double sec = _time/CLOCKS_PER_SEC;
	std::cout << " sec " << _time->tv_sec << " usec " << _time->tv_usec; // << std::endl;
}
#endif
