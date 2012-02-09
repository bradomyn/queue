/*
 * WRPacket.h
 *
 *  Created on: Jan 26, 2012
 *      Author: matthies
 */

#ifndef WRPACKET_H_
#define WRPACKET_H_

#include <string>

using std::string;

namespace queueing {

class WRPacket {
public:
	WRPacket(string name, int id, int priority, double startTime, double endTime);
	virtual ~WRPacket();

	string getName() { return _name; };

	int getID() { return _id; };
	int getPriority() { return _priority; };

	double getStartTime() { return _startTime; };
	double getEndTime() { return _endTime; };

private:
	string _name;

	int _id;
	int _priority;

	double _startTime;
	double _endTime;

};

} /* namespace queueing */
#endif /* WRPACKET_H_ */
