/*
 * WRPacket.cc
 *
 *  Created on: Jan 26, 2012
 *      Author: matthies
 */

#include "WRPacket.h"

namespace queueing {

WRPacket::WRPacket(string name, int id, int priority, double startTime, double endTime) {
	_name = name;
	_id = id;
	_priority = priority;
	_startTime = startTime;
	_endTime = endTime;
}

WRPacket::~WRPacket() {
	// TODO Auto-generated destructor stub
}

} /* namespace queueing */
