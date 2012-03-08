//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "PassiveQueue.h"
#include "Job.h"

namespace queueing {

Define_Module(PassiveQueue);

PassiveQueue::PassiveQueue()
{
    selectionStrategy = NULL;
}

PassiveQueue::~PassiveQueue()
{
    delete selectionStrategy;
}

void PassiveQueue::initialize()
{
    droppedSignal = registerSignal("dropped");
    queueingTimeSignal = registerSignal("queueingTime");
    queueLengthSignal = registerSignal("queueLength");
    emit(queueLengthSignal, 0);

    capacity = par("capacity");
    queue.setName("queue");

    fifo = par("fifo");

    selectionStrategy = SelectionStrategy::create(par("sendingAlgorithm"), this, false);
    if (!selectionStrategy)
        error("invalid selection strategy");

    WATCH(numServed);
    numServed = 0;

    WATCH(numQueued);
    numQueued = 0;

    WATCH(numQueuedIdle);
    numQueuedIdle = 0;

    const char * algName = par("schedulingAlgorithm");
    std::cout << this->getName() << " scheduling algorithm " << algName << std::endl;
    if (strcmp(algName, "none") == 0) {
		_scheduling = 0;
	} else if (strcmp(algName, "priority") == 0) {
		_scheduling = 1;
	} else if (strcmp(algName, "feedback") == 0) {
		_scheduling = 2;
	} else if (strcmp(algName, "original") == 0) {
		_scheduling = 3;
	} else if (strcmp(algName, "7first") == 0) {
		_scheduling = 4;
	}

} // initialize()

int PassiveQueue::determineQueueSize() {
	if( queue.length()>0 ) {
		int size=0;
		int queuelength = queue.length();	// TODO can the length change during size determination? I hope not!
		// iterate queue without removing or changing objects inside
		for( int i; i<queuelength; i++ ) {
			Job *j = check_and_cast<Job *>(queue.get(i));
			if( j!=NULL ) {
				size += j->getSize();
			}
		}
		return size;
	} else {
		//std::cout << "queue length 0" << std::endl;
		return 0;
	}
} // determineQueueSize()

void PassiveQueue::handleMessage(cMessage *msg)
{
    Job *job = check_and_cast<Job *>(msg);
    job->setTimestamp();
    int k;

    switch(_scheduling) {
    case 0:	// none
    	// use with WRS.ned
    	// send through without queueing
		send(job, "out", 0);
		numServed++;
    	break;
    case 1:	// prority
    	// use with WRS1.ned
    	//std::cout << this->getName() << " priority" << std::endl;
        //std::cout << "capacity " << capacity << " " << this->getName() << " queue size " << determineQueueSize() << std::endl;
        // check for container capacity
        //if (capacity >=0 && queue.length() >= capacity)
        if( capacity >=0 && determineQueueSize()>=capacity ) {
            EV << this->getName() << " full! Job dropped.\n";
            std::cerr << this->getName() << " full! Job dropped." << std::endl;
            if (ev.isGUI()) bubble("Dropped!");
            emit(droppedSignal, 1);
            delete msg;
            return;
        }
        // Trigger Test
        // queue everything until requested, truly passive queue
        queue.insert(job);
        //std::cout << "queued: " << job->getName() << std::endl;
        emit(queueLengthSignal, length());
        job->setQueueCount(job->getQueueCount() + 1);
        numQueued++;

        // change the icon color
        if (ev.isGUI())
            getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan3");
    	break;
    case 2:	// feedback
    	//std::cout << this->getName() << " feedback" << std::endl;
    	// TODO

    	break;
    case 3:	// original
    	//std::cout << this->getName() << " original" << std::endl;
    	// use with WRS.ned
    	job->setTimestamp();

		// check for container capacity
		if (capacity >=0 && queue.length() >= capacity)
		{
			EV << "Queue full! Job dropped.\n";
			if (ev.isGUI()) bubble("Dropped!");
			emit(droppedSignal, 1);
			delete msg;
			return;
		}

		k = selectionStrategy->select();
		if (k < 0)
		{
			// enqueue if no idle server found
			queue.insert(job);
			emit(queueLengthSignal, length());
			job->setQueueCount(job->getQueueCount() + 1);
		}
		else if (length() == 0)
		{
			// send through without queueing
			send(job, "out", k);
			numServed++;
		}
		else
			error("This should not happen. Queue is NOT empty and there is an IDLE server attached to us.");

		// change the icon color
		if (ev.isGUI())
			getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan3");
    	break;
	case 4:	// 7first
		// use with WRS1.ned
		//std::cout << this->getName() << " 7first" << std::endl;
		if( job->getPriority()==7 ){
			// send through without queueing
			send(job, "out",0);
			numServed++;
		} else {
			// enqueue if no idle server found
			queue.insert(job);
			emit(queueLengthSignal, length());
			job->setQueueCount(job->getQueueCount() + 1);
		}
		break;
    default:
    	break;
    } // switch
} // handleMessage()

int PassiveQueue::length()
{
    return queue.length();
} // length()

void PassiveQueue::request(int gateIndex)
{
    Enter_Method("request()!");

    ASSERT(!queue.empty());

    Job *job;
    if (fifo)
    {
        job = (Job *)queue.pop();
        //std::cout << __FUNCTION__ << " pop: " << job->getName() << std::endl;
    }
    else
    {
        job = (Job *)queue.back();
        //std::cout << __FUNCTION__ << " back: " << job->getName() << std::endl;
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(job);
    }
    emit(queueLengthSignal, length());

    job->setQueueCount(job->getQueueCount()+1);
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalQueueingTime(job->getTotalQueueingTime() + d);
    emit(queueingTimeSignal, d);

    send(job, "out", gateIndex);
    numServed++;
    std::cout << "job " << job->getName() << " requested over gate " << gateIndex << std::endl;

    if (ev.isGUI())
        getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan");
} // request(index)

void PassiveQueue::request()
{
    Enter_Method("request()!");

    ASSERT(!queue.empty());

    Job *job;
    if (fifo) {
        job = (Job *)queue.pop();
        std::cout << __FUNCTION__ << " pop: " << job->getName() << std::endl;
    } else {
        job = (Job *)queue.back();
        std::cout << __FUNCTION__ << " back: " << job->getName() << std::endl;
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(job);
    }
    emit(queueLengthSignal, length());

    job->setQueueCount(job->getQueueCount()+1);
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalQueueingTime(job->getTotalQueueingTime() + d);
    emit(queueingTimeSignal, d);

    send(job, "out");
    numServed++;
    //std::cout << "job requested without gate " << std::endl;

    if (ev.isGUI())
        getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan");
} // request()

}; //namespace

