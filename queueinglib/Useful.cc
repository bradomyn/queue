/*
 * Useful.cc
 *
 *  Created on: Feb 20, 2012
 *      Author: matthies
 */

#include "Useful.h"

namespace queueing {

queueing::Useful * queueing::Useful::_instance = 0;


Useful::Useful() {
	_instance = NULL;
}

Useful::~Useful() {
}

Useful *Useful::getInstance() {
	if( _instance==NULL ) {
		_instance = new Useful();
	}
	return _instance;
}

double Useful::getTime(std::string name) {
	size_t found;
	found = name.find("; ");
	//std::cout << "jobname " << jobname << " found " << found << std::endl;
	// extract time from jobname
	double triggerTime=0.;
	if( found!=std::string::npos ) {
		std::string time = name.substr(found+2);
		std::istringstream stm;
		stm.str(time);
		stm >> triggerTime;
		//std::cout << "time " << time << " " << triggerTime << std::endl;
	}
	return triggerTime;
}

int Useful::getPriority(std::string name) {
	size_t found1;
	found1 = name.find("y: ");
	//std::cout << "jobname " << jobname << " found1 " << found1 << std::endl;
	// extract priority from jobname
	int prio=0;
	if( found1!=std::string::npos ) {
		std::string priority = name.substr(found1+2);
		std::istringstream stm;
		stm.str(priority);
		stm >> prio;
		//std::cout << "priority " << prio << std::endl;
	}
	return prio;
} // getPriority()

int Useful::generateRandom() {
	int random = 0;
#if 0
	srand(time(NULL));
	// TODO
	random = rand() % 4 + 0;  //number between 0 and 7
#else
	random = (int)(rand() / (((double)RAND_MAX + 1)/ (double)(7+1)));
#endif
	return random;
} // generateRandom()


} /* namespace queueing */
