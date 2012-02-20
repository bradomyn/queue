/*
 * Useful.h
 *
 *  Created on: Feb 20, 2012
 *      Author: matthies
 */

#ifndef USEFUL_H_
#define USEFUL_H_

#include <string>
#include <sstream>

using namespace std;

namespace wrs {

class Useful {


public:
	Useful();
	virtual ~Useful();

	static Useful *getInstance();

	double getTime(string name);
	int getPriority(string name);

	int generateRandom();

private:
	static Useful *_instance;

};

} /* namespace queueing */
#endif /* USEFUL_H_ */