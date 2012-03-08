//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Trigger.h"

namespace queueing {


void TriggerBase::initialize() {
	createdSignal = registerSignal("created");
	triggerCounter = 0;
	WATCH(triggerCounter);
}

Job *TriggerBase::createJob() {
	Job *job = new Job("trigger");
	return job;
}

void TriggerBase::finish() {
	emit(createdSignal, triggerCounter);
}

//----

Define_Module(Trigger);

void Trigger::initialize() {
	TriggerBase::initialize();
	startTime = par("startTime");
	stopTime = par("stopTime");
	numJobs = par("numJobs");

	// schedule the first message timer for start time
	scheduleAt(startTime, new cMessage("trigger"));

	WATCH(numCreated);
	numCreated = 0;

}

void Trigger::handleMessage(cMessage *msg) {
	ASSERT(msg->isSelfMessage());

	if ((numJobs < 0 || numJobs > triggerCounter)
			&& (stopTime < 0 || stopTime > simTime())) {
		// reschedule the timer for the next message
		simtime_t triggerTime = simTime() + par("triggerIntervall").doubleValue();
		scheduleAt(triggerTime, msg);

		Job *job = generateTrigger();

		send(job, "out");
		//std::cout << "Trigger sent at " << triggerTime << std::endl;

		numCreated++;
	} else {
		// finished
		delete msg;
	}
}

Job * Trigger::generateTrigger() {
	Job *job = new Job();
	simtime_t creationTime = simTime();
	job->setName("trigger");
	job->setTimestamp(creationTime);
	return job;
} // generateTrigger()

};
//namespace

