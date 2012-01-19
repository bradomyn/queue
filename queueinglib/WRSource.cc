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
	createdSignal = registerSignal("created");

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
    stopTime = par("stopTime");
    numJobs = par("numJobs");

    // schedule the first message timer for start time
    scheduleAt(startTime, new cMessage("newJobTimer"));

    /*cMessage *msg = new cMessage("newJobTimer");
	rtScheduler = check_and_cast<cSocketRTScheduler *>(simulation.getScheduler());
    rtScheduler->setInterfaceModule(this, msg, recvBuffer, 4000, &numRecvBytes);
*/
}

void WRSource::handleMessage(cMessage *msg)
{
    ASSERT(msg->isSelfMessage());

    //should be done in a main method before setting up the network
    //cRealTimeScheduler *rts = new cRealTimeScheduler();
    //simulation.setScheduler(rts);

    if ((numJobs < 0 || numJobs > jobCounter) && (stopTime < 0 || stopTime > simTime()))
    {
        // reschedule the timer for the next message
    	//scheduleAt(simTime() + par("interArrivalTime").doubleValue(), msg);
    	// SMa, 12.01.2012
        scheduleAt(simTime()+ par("interArrivalTime").doubleValue(), msg);

        Job *job = createJob();

        // generate random job priorities between 0..7
        //int random = 0 + rand()%(7-0+1);
        int random = (int)(rand() / (((double)RAND_MAX + 1)/ (double)(7+1)));

        // TODO work with a fixed, repeatable data set
        job->setPriority(random);
        char name[25];
        sprintf(name, "id: %ld, priority: %d", job->getId(), random);
        job->setName(name);
        std::cout << "job (id: " << job->getId() << ") priority set to: " << random << std::endl;

        send(job, "out");
    }
    else
    {
        // finished
        delete msg;
    }

}

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

