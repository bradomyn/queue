//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Source.h"

namespace queueing {


void SourceBase::initialize() {
	createdSignal = registerSignal("created");
	jobCounter = 0;
	WATCH(jobCounter);
	jobName = par("jobName").stringValue();
	if (jobName == "")
		jobName = getName();
}

Job *SourceBase::createJob() {
	char buf[80];
	sprintf(buf, "%.60s-%d", jobName.c_str(), ++jobCounter);
	Job *job = new Job(buf);
	//job->setKind(par("jobType"));
	int prio = Useful::getInstance()->generateRandomPriority();
	job->setPriority(prio); //par("jobPriority"));
	return job;
}

void SourceBase::finish() {
	emit(createdSignal, jobCounter);
}

//----

Define_Module(Source);

void Source::initialize() {
	SourceBase::initialize();
	startTime = par("startTime");
	stopTime = par("stopTime");
	numJobs = par("numJobs");

	// schedule the first message timer for start time
	scheduleAt(startTime, new cMessage("newJobTimer"));

	_data = Useful::getInstance()->readDataList("data_10000.txt");

	//for( int i=0; i<_data.size(); i++ )
	  //std::cout << _data.at(i).getPriority() << " " << _data.at(i).getSize() << std::endl;


	WATCH(numCreated);
	numCreated = 0;
}

void Source::handleMessage(cMessage *msg) {
	ASSERT(msg->isSelfMessage());

#if 1

	if( (numJobs < 0 || numJobs > jobCounter || numJobs<_data.size() )
			&& (stopTime < 0 || stopTime > simTime()) ) {
		// reschedule the timer for the next message
		simtime_t sourceTime = simTime() + par("interArrivalTime").doubleValue();
		scheduleAt(sourceTime, msg);

		Job *job = generateJob( _data.at(numCreated).getPriority(), _data.at(numCreated).getSize() );
		send(job, "out");
		//std::cout << "Message sent at " << sourceTime << std::endl;
		numCreated++;
	} else {
		std::cout << "No more packets available." << std::endl;
		// finished
		delete msg;
	}
#else
	if ((numJobs < 0 || numJobs > jobCounter)
			&& (stopTime < 0 || stopTime > simTime())) {
		// reschedule the timer for the next message
		simtime_t sourceTime = simTime() + par("interArrivalTime").doubleValue();
		scheduleAt(sourceTime, msg);

		Job *job = generateJob(); //createJob();
		send(job, "out");
		//std::cout << "Message sent at " << sourceTime << std::endl;

		numCreated++;
	} else {
		// finished
		delete msg;
	}
#endif

}

Job * Source::generateJob() {
	//log("test");
	char buf[80];
	std::string jobName = "j";
	sprintf(buf, "%.60s-%d", jobName.c_str(), ++jobCounter);
	Job *job = new Job(buf);
	int randomP = Useful::getInstance()->generateRandomPriority();

	// TODO work with a fixed, repeatable data set
	job->setPriority(randomP);

	int randomS = Useful::getInstance()->generateRandomSize();

	// TODO work with a fixed, repeatable data set
	job->setSize(randomS);

	// to build a data file:
	//Useful::getInstance()->writeRandomDataToList("data.txt", randomP, randomS);


	simtime_t creationTime = simTime();
	char name[80];
	//sprintf(name, "id: %ld, priority: %d; %f", job->getId(), random, triggerTime);
	sprintf(name, "id: %ld, priority: %d; > %lf", job->getId(), randomP, creationTime.dbl());
	name[79] = '\0';
	job->setName(name);
	std::cout << "job (id: " << job->getId() << ") priority set to: " << randomP << " size " << randomS << std::endl;

	job->setTimestamp(creationTime);

	return job;
} // generateJob()

Job * Source::generateJob( int priority, int size ) {
	//log("test");
	char buf[80];
	std::string jobName = "j";
	sprintf(buf, "%.60s-%d", jobName.c_str(), ++jobCounter);

	Job *job = new Job(buf);
	job->setPriority(priority);
	job->setSize(size);

	simtime_t creationTime = simTime();
	char name[80];
	//sprintf(name, "id: %ld, priority: %d; %f", job->getId(), random, triggerTime);
	sprintf(name, "id: %ld, priority: %d; > %lf", job->getId(), priority, creationTime.dbl());
	name[79] = '\0';
	job->setName(name);
	//std::cout << "job (id: " << job->getId() << ") priority set to: " << priority << " size " << size << std::endl;

	job->setTimestamp(creationTime);

	return job;
} // generateJob()

};
//namespace

