//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "WRSource.h"
#include "Job.h"
#include <cscheduler.h>

namespace queueing {


void WRSourceBase::initialize()
{
	//createdSignal = registerSignal("created");

	jobCounter = 0;
    WATCH(jobCounter);
    jobName = par("jobName").stringValue();
    if (jobName == "")
        jobName = getName();
}

Job *WRSourceBase::createJob()
{
    char buf[80];
    sprintf(buf, "%.60s-%d", jobName.c_str(), ++jobCounter);
    Job *job = new Job(buf);
    // SMa, 12.01.2012
    job->setKind(par("jobType").longValue());
    job->setPriority(par("jobPriority").longValue());
    return job;
}

void WRSourceBase::finish()
{
    emit(createdSignal, jobCounter);
}

//----

Define_Module(WRSource);

void WRSource::initialize()
{
    WRSourceBase::initialize();
    startTime = par("startTime");
    //stopTime = par("stopTime");
    numJobs = par("numJobs");

    // schedule the first message timer for start time
    scheduleAt(startTime, new cMessage("newJobTimer"));

}

// TODO how to schedule them to happen at the same time?
// TODO also the queues should be handled at the same time
void WRSource::handleMessage(cMessage *msg)
{
    if ((numJobs < 0 || numJobs > jobCounter) )  // && (stopTime < 0 || stopTime > simTime()))
    {
        // reschedule the timer for the next message
        //scheduleAt(simTime() /*+ par("interArrivalTime").doubleValue()*/, msg);
#if 0
        //Job *job = createJob();
    	Job *job = generateJob();
    	cModule *targetModule = getParentModule()->getSubmodule("classifier");
    	sendDirect(job, targetModule, "sendDirect");
#else

#if 0
    	// create more jobs per run
    	for( int i=0; i<10; i++ ) {
    		//std::cout << "generate job " << i << std::endl;
    	    Job *job = generateJob();
    	    //std::cout << " created: " << job->getName() << std::endl;
    	    //send(job, "outv", i);

    	    cModule *targetModule = getParentModule()->getSubmodule("classifier");
    	    sendDirect(job, targetModule, "sendDirect");
    	}
#else
    	int num=5;
    	std::vector<Job*> v = generateJobs(num);
    	std::vector<Job*>::iterator it;
    	for( it=v.begin(); it!=v.end(); it++ ) {
    		cModule *targetModule = getParentModule()->getSubmodule("classifier");
    		sendDirect(*it, targetModule, "sendDirect");
    	}
    	std::cout << this->getName() << " sent " << num << " jobs: ";
    	for( it=v.begin(); it!=v.end(); it++ ) {
    		std::cout << (*it)->getId() << ", ";
    	}
    	std::cout << std::endl;
#endif
#endif
    } else {
        // finished
        delete msg;
    }
} // handleMessage()

std::vector<Job*> WRSource::generateJobs(int number) {
	std::vector<Job*> jobs;

	for( int i=0; i<number; i++ ) {
		jobs.push_back(generateJob());
	}

	return jobs;
} // generateJobs()

Job * WRSource::generateJob() {
	char buf[80];
	sprintf(buf, "%.60s-%d", jobName.c_str(), ++jobCounter);
	Job *job = new Job(buf);
	int random = (int)(rand() / (((double)RAND_MAX + 1)/ (double)(7+1)));

	// TODO work with a fixed, repeatable data set
	job->setPriority(random);

	Timer t;
	timeval tv = t.currentTime();
	triggerTime = static_cast<double>( tv.tv_sec ) + static_cast<double>( tv.tv_usec )/1E6;
	//std::cout << "TRIGGER "; t.print(); std::cout << std::endl;

	char name[80];
	sprintf(name, "id: %ld, priority: %d; %f", job->getId(), random, triggerTime);
	name[79] = '\0';
	job->setName(name);
	//std::cout << "job (id: " << job->getId() << ") priority set to: " << random << std::endl;

	return job;
} // generateJob()

//----

Define_Module(WRSourceOnce);

void WRSourceOnce::initialize()
{
    WRSourceBase::initialize();
    simtime_t time = par("time");
    scheduleAt(time, new cMessage("newJobTimer"));

    // SMa, 13.01.2012
    counter=10;
    WATCH(counter);	// allows to watch this variable in the GUI
}

void WRSourceOnce::handleMessage(cMessage *msg)
{
    ASSERT(msg->isSelfMessage());
    delete msg;

    int n = par("numJobs");
    for (int i=0; i<n; i++)
    {
        Job *job = createJob();
        send(job, "out");
        // SMa, 13.01.2012
        counter--;
    }
}

}; //namespace

