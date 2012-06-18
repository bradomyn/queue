//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "PassiveQueue.h"


namespace queueing {

Define_Module(PassiveQueue);

PassiveQueue::PassiveQueue()
{
}

PassiveQueue::~PassiveQueue()
{
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

    const char * algName = par("schedulingAlgorithm");
	std::cout << this->getName() << " scheduling algorithm " << algName << std::endl;

	if( (strcmp(algName, "original") == 0) || (strcmp(algName, "priority") == 0) || (strcmp(algName, "feedback") == 0) || (strcmp(algName, "wfq1") == 0) || (strcmp(algName, "wfq2") == 0) || (strcmp(algName, "wfq3") == 0) || (strcmp(algName, "wfq4") == 0) ){
		_scheduling = 0;
		std::cout << "queue original priority | feedback | wfq1 | wfq2 | wfq3 | wfq4" << endl;
	} else if ( (strcmp(algName, "7first") == 0) || (strcmp(algName, "mixed1") == 0) ) {
		_scheduling = 1;
		std::cout << "server 7first" << endl;
	} else if ( strcmp(algName, "fcfs") == 0 ) {
		_scheduling = 2;	// ???
		std::cout << "server fcfs" << endl;
	}

    std::cout << this->getName() << endl;
}

void PassiveQueue::finish()
{
    //std::cout << this->getName() <<  " " << queue.length() << endl;
}

void PassiveQueue::handleMessage(cMessage *msg)
{
	//std::cout << " " << __FILE__ << ": "<< __FUNCTION__ << " " << msg->getName() << std::endl;
	Packet *job = check_and_cast<Packet *>(msg);
	job->setTimestamp();
	cModule *server = NULL;

	switch( _scheduling ) {
	case 0:	// diverse
	    enqueue(msg);
		break;
	case 1:	// 7first | mixed1
		if( job->getPriority()==7 ) {
			send(job, "out", 0);
		} else {
			enqueue(msg);
		}
		break;
	case 2:	// fcfs
		enqueue(msg);

		// write priority to vector in server
		server = (cModule*)getParentModule()->findObject("server", true);
		(check_and_cast<Server *>(server))->add2QueueServeList(job->getPriority());
		break;
	default: break;
	}

    // change the icon color
    if (ev.isGUI())
        getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan3");
}

void PassiveQueue::enqueue(cMessage* msg) {
	Packet *job = check_and_cast<Packet *>(msg);
	job->setTimestamp();

	// check for container capacity
	if (capacity >= 0 && queue.length() >= capacity) {
		_dropped.push_back(job);
		EV << "Queue full! Packet dropped.\n";
		if (ev.isGUI())
			bubble("Dropped!");
		emit(droppedSignal, 1);
		delete msg;
		return;
	}

	// queue everything
	queue.insert(job);
	emit(queueLengthSignal, length());
	job->setQueueCount(job->getQueueCount() + 1);
	//std::cout << this->getName() << " Q size " << queue.length() << std::endl;
} // enequeue()

int PassiveQueue::length()
{
    return queue.length();
}

int PassiveQueue::size()
{
	int size = 0;
	for( int i=0; i<length(); i++ ) {
		size += check_and_cast<Packet *>(queue.get(i))->getSize();
	}
    return size;
}

void PassiveQueue::request(int gateIndex)
{
    Enter_Method("request()!");

    ASSERT(!queue.empty());

    Packet *job;
    if (fifo) {
        job = (Packet *)queue.pop();
    }
    else {
        job = (Packet *)queue.back();
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(job);
    }
    emit(queueLengthSignal, length());

    job->setOperationCounter( job->getOperationCounter()+queue.length() );

    job->setQueueCount(job->getQueueCount()+1);
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalQueueingTime(job->getTotalQueueingTime() + d);
    emit(queueingTimeSignal, d);

    send(job, "out", gateIndex);

    if (ev.isGUI())
        getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan");
}

}; //namespace

