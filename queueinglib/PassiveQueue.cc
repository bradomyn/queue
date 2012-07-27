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

    _capacity = par("capacity");
    _size = par("size");
    queue.setName("queue");

    fifo = par("fifo");

    const char * algName = par("schedulingAlgorithm");
	std::cout << this->getName() << " scheduling algorithm " << algName << std::endl;

	if( (strcmp(algName, "original") == 0) || (strcmp(algName, "priority") == 0) || (strcmp(algName, "lqf") == 0) || (strcmp(algName, "wfq1") == 0) || (strcmp(algName, "wfq2") == 0) || (strcmp(algName, "wfq3") == 0) || (strcmp(algName, "wfq4") == 0) ){
		_scheduling = 0;
		std::cout << "queue original priority | lqf | wfq1 | wfq2 | wfq3 | wfq4" << endl;
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
	WRPacket *job = check_and_cast<WRPacket *>(msg);
	job->setTimestamp();
	cModule *server = NULL;

	switch( _scheduling ) {
	case 0:	// diverse
	    enqueue(job);
	    //send(job, "out", 0);
		break;
	case 1:	// 7first | mixed1
		if( job->getPriority()==7 ) {
			send(job, "out", 0);
		} else {
			enqueue(job);
		}
		break;
	case 2:	// fcfs
		enqueue(job);

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

void PassiveQueue::enqueue(cPacket* msg) {
	WRPacket *job = check_and_cast<WRPacket *>(msg);
	job->setTimestamp();

	// check for container capacity
	if ((_capacity >= 0) && (queue.length() >= _capacity) && (size()>_size)) {
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

	// trigger server once
	cMessage* trigger = new cMessage("startServiceMessage");
	send(trigger, "outTrigger");

} // enequeue()

int PassiveQueue::length()
{
    return queue.length();
}

int PassiveQueue::size()
{
	int size = 0;
	for( int i=0; i<length(); i++ ) {
		size += check_and_cast<WRPacket *>(queue.get(i))->getByteLength();
	}
    return size;
}

void PassiveQueue::request(int gateIndex)
{
    Enter_Method("request()!");

    ASSERT(!queue.empty());
    WRPacket *job;
    if (fifo) {
        job = (WRPacket *)queue.pop();
    }
    else {
        job = (WRPacket *)queue.back();
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(job);
    }
    emit(queueLengthSignal, length());

    job->setOperationCounter( job->getOperationCounter()+queue.length() );

    job->setQueueCount(job->getQueueCount()+1);
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalQueueingTime(job->getTotalQueueingTime() + d);
    //cout << "queuingTime " << job->getTotalQueueingTime() << endl;
    emit(queueingTimeSignal, d);

    send(job, "out", gateIndex);

    if (ev.isGUI())
        getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan");
}

WRPacket * PassiveQueue::front() {
	return (WRPacket *)queue.front();
} // at

}; //namespace

