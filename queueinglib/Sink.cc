//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Sink.h"
#include "Packet.h"
#include "IPassiveQueue.h"

namespace queueing {

Define_Module(Sink);

void Sink::initialize()
{
    lifeTimeSignal = registerSignal("lifeTime");
    totalQueueingTimeSignal = registerSignal("totalQueueingTime");
    queuesVisitedSignal = registerSignal("queuesVisited");
    totalServiceTimeSignal = registerSignal("totalServiceTime");
    totalDelayTimeSignal = registerSignal("totalDelayTime");
    delaysVisitedSignal = registerSignal("delaysVisited");
    generationSignal = registerSignal("generation");
    keepPackets = par("keepPackets");

    numReceived = 0;
    WATCH(numReceived);
}

void Sink::handleMessage(cMessage *msg)
{
	simtime_t currentTime = simTime();
	simtime_t lifetime = currentTime-msg->getCreationTime();
    Packet *packet = check_and_cast<Packet *>(msg);

    numReceived++;

    std::cout << "#" << numReceived << ": " << packet->getName() << " received. ct " << msg->getCreationTime() << " st " << currentTime << " lifetime " << lifetime  << std::endl;

    switch( packet->getPriority() ) {
    case 0:
    	v0.push_back(lifetime);
    	break;
    case 1:
    	v1.push_back(lifetime);
        	break;
    case 2:
    	v2.push_back(lifetime);
        	break;
    case 3:
    	v3.push_back(lifetime);
        	break;
    case 4:
    	v4.push_back(lifetime);
        	break;
    case 5:
    	v5.push_back(lifetime);
        	break;
    case 6:
    	v6.push_back(lifetime);
        	break;
    case 7:
    	v7.push_back(lifetime);
        	break;
    default:
    	break;
    }


    // gather statistics
    emit(lifeTimeSignal, simTime()- packet->getCreationTime());
    emit(totalQueueingTimeSignal, packet->getTotalQueueingTime());
    emit(queuesVisitedSignal, packet->getQueueCount());
    emit(totalServiceTimeSignal, packet->getTotalServiceTime());
    emit(totalDelayTimeSignal, packet->getTotalDelayTime());
    emit(delaysVisitedSignal, packet->getDelayCount());
    emit(generationSignal, packet->getGeneration());

    if (!keepPackets)
        delete msg;
}

double Sink::avg_lifetime(vector<simtime_t> v) {
	double avg_lt = 0.;
	std::vector<simtime_t>::iterator lit;
	for( lit = v.begin(); lit!=v.end(); lit++ ) {
		avg_lt += (*lit).dbl();
	}
	avg_lt /= v.size();
	//std:: cout << "    lifetime " << avg_lt << "s = " << avg_lt / 1000.0 << "ms" << std::endl;
	return avg_lt;
}

void Sink::finish()
{
    // TODO missing scalar statistics

	std::cout << "p 0: avg " << avg_lifetime(v0) << " size " << v0.size() << std::endl;
	std::cout << "p 1: avg " << avg_lifetime(v1) << " size " << v1.size() << std::endl;
	std::cout << "p 2: avg " << avg_lifetime(v2) << " size " << v2.size() << std::endl;
	std::cout << "p 3: avg " << avg_lifetime(v3) << " size " << v3.size() << std::endl;
	std::cout << "p 4: avg " << avg_lifetime(v4) << " size " << v4.size() << std::endl;
	std::cout << "p 5: avg " << avg_lifetime(v5) << " size " << v5.size() << std::endl;
	std::cout << "p 6: avg " << avg_lifetime(v6) << " size " << v6.size() << std::endl;
	std::cout << "p 7: avg " << avg_lifetime(v7) << " size " << v7.size() << std::endl;

	determineQueueSizes();
}

void Sink::determineQueueSizes() {

	std::string queue;
	char buffer[3];

	for( int i=0; i<8; i++ ) {
		queue = "passiveQueue";
		sprintf(buffer,"%d",i);
		buffer[2]='\0';
		queue += buffer;
		cModule *module = getParentModule()->getSubmodule(queue.c_str());
		IPassiveQueue *pqueue = dynamic_cast<IPassiveQueue *>(module);
		if (pqueue != NULL)
			std::cout << queue.c_str() << " length " <<  pqueue->length() << std::endl;
	}
}

}; //namespace

