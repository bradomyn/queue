//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "PassiveQueue.h"
#include "Packet.h"

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

    _capacity = par("capacity");
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
		std::cout << "queue none" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue none");
	} else if (strcmp(algName, "priority") == 0) {
		_scheduling = 1;
		std::cout << "queue priority" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue priority");
	} else if (strcmp(algName, "feedback1") == 0) {
		_scheduling = 2;
		std::cout << "queue feedback" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue feedback");
	} else if (strcmp(algName, "original") == 0) {
		_scheduling = 3;
		std::cout << "queue original" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue original");
	} else if (strcmp(algName, "7first") == 0) {
		_scheduling = 4;
		std::cout << "queue 7first" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue 7first");
	} else if (strcmp(algName, "feedback2") == 0) {
		_scheduling = 5;
		std::cout << "queue feedback2" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue feedback2");
	} else if (strcmp(algName, "feedback3") == 0) {
		_scheduling = 6;
		std::cout << "queue feedback3" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue feedback3");
	} else if ((strcmp(algName, "wfq1") == 0) || strcmp(algName, "wfq4") == 0) {
		_scheduling = 7;
		std::cout << "queue wfq1/4" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue wfq1/4");
	} else if (strcmp(algName, "mixed") == 0) {
		_scheduling = 8;
		std::cout << "queue mixed" << std::endl;
		Useful::getInstance()->appendToFileTab("out.txt", "queue mixed");
	}

} // initialize()

int PassiveQueue::determineQueueSize() {
	if( queue.length()>0 ) {
		int size=0;
		int queuelength = queue.length();	// TODO can the length change during size determination? I hope not!
		// iterate queue without removing or changing objects inside
		for( int i; i<queuelength; i++ ) {
			Packet *j = check_and_cast<Packet *>(queue.get(i));
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
    Packet *packet = check_and_cast<Packet *>(msg);
    packet->setTimestamp();
    int k;

    switch(_scheduling) {
    case 0:	// none
    	// use with WRS.ned
    	// send through without queueing
		send(packet, "out", 0);
		numServed++;
    	break;
    case 1:	// prority
    	// use with WRS1.ned
    	//std::cout << this->getName() << " priority" << std::endl;
        //std::cout << "capacity " << capacity << " " << this->getName() << " queue size " << determineQueueSize() << std::endl;
    	checkCapacityAndQueue(msg);
    	break;
    case 2:	// feedback
    	//std::cout << this->getName() << " feedback" << std::endl;
    	//checkCapacityAndQueue(msg);
    	send(packet, "out", 0);
    	numServed++;
    	break;
    case 3:	// original
    	//std::cout << this->getName() << " original" << std::endl;
    	// use with WRS.ned
    	packet->setTimestamp();

		// check for container capacity
		if (_capacity >=0 && queue.length() >= _capacity) {
			EV << "Queue full! Packet dropped.\n";
			if (ev.isGUI()) bubble("Dropped!");
			emit(droppedSignal, 1);
			delete msg;
			return;
		}

		k = selectionStrategy->select();
		if (k < 0) {
			// enqueue if no idle server found
			queue.insert(packet);
			emit(queueLengthSignal, length());
			packet->setQueueCount(packet->getQueueCount() + 1);
		} else if (length() == 0) {
			// send through without queueing
			send(packet, "out", k);
			numServed++;
		} else
			error("This should not happen. Queue is NOT empty and there is an IDLE server attached to us.");

		// change the icon color
		if (ev.isGUI())
			getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan3");
    	break;
	case 4:	// 7first
		// use with WRS1.ned
		//std::cout << this->getName() << " 7first" << std::endl;
		if( packet->getPriority()==7 ){
			// send through without queueing
			send(packet, "out",0);
			numServed++;
		} else {
			// enqueue if no idle server found
			queue.insert(packet);
			emit(queueLengthSignal, length());
			packet->setQueueCount(packet->getQueueCount() + 1);
		}
		break;
	case 5:	// feedback 2
		//checkCapacityAndQueue(msg);
		send(packet, "out", 0);
		numServed++;
		break;
	case 6:	// feedback 3
		send(packet, "out", 0);
		numServed++;
		break;
	case 7:	// wfq1/4
		send(packet, "out", 0);
		numServed++;
		break;
	case 8:	// mixed
		send(packet, "out", 0);
		numServed++;
		break;
    default:
    	break;
    } // switch
} // handleMessage()

void PassiveQueue::checkCapacityAndQueue(cMessage *msg) {
	Packet *packet = check_and_cast<Packet *>(msg);
	if( _capacity >=0 && determineQueueSize()>=_capacity ) {
		EV << this->getName() << " full! Packet dropped.\n";
		std::cerr << this->getName() << " full! Packet dropped." << std::endl;
		if (ev.isGUI()) bubble("Dropped!");
		emit(droppedSignal, 1);
		delete msg;
		return;
	}
	// Trigger Test
	// queue everything until requested, truly passive queue
	queue.insert(packet);
	//std::cout << "queued: " << packet->getName() << std::endl;
	emit(queueLengthSignal, length());
	packet->setQueueCount(packet->getQueueCount() + 1);
	numQueued++;

	// change the icon color
	if (ev.isGUI())
		getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan3");
} // checkCapacityAndQueue()

int PassiveQueue::length()
{
    return queue.length();
} // length()

void PassiveQueue::request(int gateIndex)
{
    Enter_Method("request()!");

    ASSERT(!queue.empty());

    Packet *packet;
    if (fifo)
    {
        packet = (Packet *)queue.pop();
        //std::cout << __FUNCTION__ << " pop: " << packet->getName() << std::endl;
    }
    else
    {
        packet = (Packet *)queue.back();
        //std::cout << __FUNCTION__ << " back: " << packet->getName() << std::endl;
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(packet);
    }
    emit(queueLengthSignal, length());

    packet->setQueueCount(packet->getQueueCount()+1);
    simtime_t d = simTime() - packet->getTimestamp();
    packet->setTotalQueueingTime(packet->getTotalQueueingTime() + d);
    emit(queueingTimeSignal, d);

    send(packet, "out", gateIndex);
    numServed++;
    //std::cout << "packet " << packet->getName() << " requested over gate " << gateIndex << std::endl;

    if (ev.isGUI())
        getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan");
} // request(index)

void PassiveQueue::request()
{
    Enter_Method("request()!");

    ASSERT(!queue.empty());

    Packet *packet;
    if (fifo) {
        packet = (Packet *)queue.pop();
        std::cout << __FUNCTION__ << " pop: " << packet->getName() << std::endl;
    } else {
        packet = (Packet *)queue.back();
        std::cout << __FUNCTION__ << " back: " << packet->getName() << std::endl;
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(packet);
    }
    emit(queueLengthSignal, length());

    packet->setQueueCount(packet->getQueueCount()+1);
    simtime_t d = simTime() - packet->getTimestamp();
    packet->setTotalQueueingTime(packet->getTotalQueueingTime() + d);
    emit(queueingTimeSignal, d);

    send(packet, "out");
    numServed++;
    //std::cout << "packet requested without gate " << std::endl;

    if (ev.isGUI())
        getDisplayString().setTagArg("i",1, queue.empty() ? "" : "cyan");
} // request()

}; //namespace

