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

namespace queueing {

Define_Module(Sink);

void Sink::initialize() {
	lifeTimeSignal = registerSignal("lifeTime");
	totalQueueingTimeSignal = registerSignal("totalQueueingTime");
	queuesVisitedSignal = registerSignal("queuesVisited");
	totalServiceTimeSignal = registerSignal("totalServiceTime");
	totalDelayTimeSignal = registerSignal("totalDelayTime");
	delaysVisitedSignal = registerSignal("delaysVisited");
	generationSignal = registerSignal("generation");
	keepJobs = par("keepJobs");

	_nofCoS = par("nofCoS");

	numReceived = 0;
	WATCH(numReceived);

    // pointers to other queues
    //for( int i=_nofCoS; i>-1; i-- )
	for( int i=0; i<_nofCoS;i++ )
    	_qs.push_back( getQueue(i) );
}

PassiveQueue *Sink::getQueue(int index) {
	std::string queue = "queue";
	char buffer[3];

	sprintf(buffer,"%d",index);
	buffer[2]='\0';
	queue += buffer;
	cModule *module = getParentModule()->getSubmodule(queue.c_str());
	PassiveQueue *pqueue = dynamic_cast<PassiveQueue *>(module);

	return pqueue;
} // getQueue()

void Sink::handleMessage(cMessage *msg) {

	if (strcmp(msg->getName(), "trigger") != 0) {
		simtime_t currentTime = simTime();
		simtime_t lifetime = currentTime - msg->getCreationTime();
		Packet *packet = check_and_cast<Packet *>(msg);

		numReceived++;
		switch (packet->getPriority()) {
		case 0:
			v0.push_back(lifetime);
			v00q.push_back(packet->getOperationCounter() * 0.000000008);
			vq0.push_back(packet->getTotalQueueingTime());
			break;
		case 1:
			v1.push_back(lifetime);
			v01q.push_back(packet->getOperationCounter() * 0.000000008);
			vq1.push_back(packet->getTotalQueueingTime());
			break;
		case 2:
			v2.push_back(lifetime);
			v02q.push_back(packet->getOperationCounter() * 0.000000008);
			vq2.push_back(packet->getTotalQueueingTime());
			break;
		case 3:
			v3.push_back(lifetime);
			v03q.push_back(packet->getOperationCounter() * 0.000000008);
			vq3.push_back(packet->getTotalQueueingTime());
			break;
		case 4:
			v4.push_back(lifetime);
			v04q.push_back(packet->getOperationCounter() * 0.000000008);
			vq4.push_back(packet->getTotalQueueingTime());
			break;
		case 5:
			v5.push_back(lifetime);
			v05q.push_back(packet->getOperationCounter() * 0.000000008);
			vq5.push_back(packet->getTotalQueueingTime());
			break;
		case 6:
			v6.push_back(lifetime);
			v06q.push_back(packet->getOperationCounter() * 0.000000008);
			vq6.push_back(packet->getTotalQueueingTime());
			break;
		case 7:
			v7.push_back(lifetime);
			v07q.push_back(packet->getOperationCounter() * 0.000000008);
			vq7.push_back(packet->getTotalQueueingTime());
			break;
		default:
			break;
		}

		// gather statistics
		emit(lifeTimeSignal, simTime() - packet->getCreationTime());
		emit(totalQueueingTimeSignal, packet->getTotalQueueingTime());
		emit(queuesVisitedSignal, packet->getQueueCount());
		emit(totalServiceTimeSignal, packet->getTotalServiceTime());
		emit(totalDelayTimeSignal, packet->getTotalDelayTime());
		emit(delaysVisitedSignal, packet->getDelayCount());
		emit(generationSignal, packet->getGeneration());

		if (!keepJobs)
			delete msg;
	}
}

void Sink::finish() {

	cModule *module = getParentModule()->getSubmodule("source");
	Source *psource = dynamic_cast<Source *>(module);

	int qs_size = _qs.size();

	// overview
	cout << "CoS " << _qs.size() << endl;

	std::cout << "p 0: avg " << avg_lifetime(v0) << " size " << v0.size() << " Q time: " << avg_lifetime(vq0) << " sent: " << psource->getSent().at(0) << " dropped " << _qs.at(0)->getDropped().size() << std::endl;
	std::cout << "p 1: avg " << avg_lifetime(v1) << " size " << v1.size() << " Q time: " << avg_lifetime(vq1) << " sent: " << psource->getSent().at(1) << " dropped " << _qs.at(1)->getDropped().size() << std::endl;
	std::cout << "p 2: avg " << avg_lifetime(v2) << " size " << v2.size() << " Q time: " << avg_lifetime(vq2) << " sent: " << psource->getSent().at(2) << " dropped " << _qs.at(2)->getDropped().size() << std::endl;
	std::cout << "p 3: avg " << avg_lifetime(v3) << " size " << v3.size() << " Q time: " << avg_lifetime(vq3) << " sent: " << psource->getSent().at(3) << " dropped " << _qs.at(3)->getDropped().size() << std::endl;
	std::cout << "p 4: avg " << avg_lifetime(v4) << " size " << v4.size() << " Q time: " << avg_lifetime(vq4) << " sent: " << psource->getSent().at(4) << " dropped " << _qs.at(4)->getDropped().size() << std::endl;

	if( _qs.size()>=6 ) {
		std::cout << "p 5: avg " << avg_lifetime(v5) << " size " << v5.size() << " Q time: " << avg_lifetime(vq5) << " sent: " << psource->getSent().at(5) << " dropped " << _qs.at(5)->getDropped().size() << std::endl;
	}
	if( _qs.size()>=7 ) {
		std::cout << "p 6: avg " << avg_lifetime(v6) << " size " << v6.size() << " Q time: " << avg_lifetime(vq6) << " sent: " << psource->getSent().at(6) << " dropped " << _qs.at(6)->getDropped().size() << std::endl;
	}
	if( _qs.size()==8 ) {
		std::cout << "p 7: avg " << avg_lifetime(v7) << " size " << v7.size() << " Q time: " << avg_lifetime(vq7) << " sent: " << psource->getSent().at(7) << " dropped " << _qs.at(7)->getDropped().size() << std::endl;
	}
	determineQueueSizes();

	// find out number of objects still stuck in server
	cModule *server = (cModule*)getParentModule()->findObject("server", true);
	int nofObjects = (check_and_cast<Server *>(server))->getLiveObjectCount();
	cout << " " << nofObjects << std::endl;
	//int nofDropped = (check_and_cast<Server *>(server))->getDropped().size();
	//cout << "dropped " << nofDropped;

	cModule *source = (cModule*)getParentModule()->findObject("source", true);
	int nofCreated = (check_and_cast<Source *>(source))->getCreated();
	int nofArrived = v0.size()+v1.size()+v2.size()+v3.size()+v4.size()+v5.size()+v6.size()+v7.size();
	cout << " created: " << nofCreated  << " arrived: " << nofArrived << endl;

	string str;
	char buf[50];

	int index = qs_size;
	//cout << "huhu1 " << _nofCoS << " " << _qs.size() << " " << psource->getSent().size() << endl;
	// create comma separated text file for easy evaluation
	Useful::getInstance()->appendToFile("out.csv", (check_and_cast<Server *>(server))->getSchedulingAlgorithm());
	Useful::getInstance()->appendToFile("out.csv", psource->getInputDataFileName());
	if( _nofCoS==8 ) {
		sprintf(buf,"%E,%d,%d", avg_lifetime(vq7), psource->getSent().at(7), _qs.at(7-1)->getDropped().size());
		str = string(buf);
		Useful::getInstance()->appendToFile("out.csv", str);
	}
	if( _nofCoS>=7 ) {
		//cout << "huhu2 " << endl;
		sprintf(buf,"%E,%d,%d", avg_lifetime(vq6), psource->getSent().at(6), _qs.at(6-1)->getDropped().size());
		str = string(buf);
		Useful::getInstance()->appendToFile("out.csv", str);
	}
	if( _nofCoS>=6 ) {
		sprintf(buf, "%E,%d,%d", avg_lifetime(vq5), psource->getSent().at(5), _qs.at(5-1)->getDropped().size());
		str = string(buf);
		Useful::getInstance()->appendToFile("out.csv", str);
	}
	//cout << "huhu3 " << endl;
	sprintf(buf, "%E,%d,%d", avg_lifetime(vq4), psource->getSent().at(4), _qs.at(4-1)->getDropped().size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.csv", str);
	sprintf(buf, "%E,%d,%d", avg_lifetime(vq3), psource->getSent().at(3), _qs.at(3-1)->getDropped().size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.csv", str);
	sprintf(buf, "%E,%d,%d", avg_lifetime(vq2), psource->getSent().at(2), _qs.at(2-1)->getDropped().size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.csv", str);
	sprintf(buf, "%E,%d,%d", avg_lifetime(vq1), psource->getSent().at(1), _qs.at(1-1)->getDropped().size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.csv", str);
	sprintf(buf, "%E,%d,%d", avg_lifetime(vq0), psource->getSent().at(0), _qs.at(0)->getDropped().size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.csv", str);


	// remove all indisposed messages in the end!
	this->setPerformFinalGC(true);
}

double Sink::avg_lifetime(vector<double> v) {
	if( v.size()>0 ) {
		double avg_lt = 0.;
		std::vector<double>::iterator lit;
		for (lit = v.begin(); lit != v.end(); lit++) {
			avg_lt += (*lit);
		}
		avg_lt /= v.size();
		//std:: cout << "    lifetime " << avg_lt << "s = " << avg_lt / 1000.0 << "ms" << std::endl;
		return (double) avg_lt;
	} else {
		return 0.0;
	}
} // avg_lifetime()

double Sink::avg_lifetime(vector<simtime_t> v) {
	if( v.size()>0 ) {
		double avg_lt = 0.;
		std::vector<simtime_t>::iterator lit;
		for (lit = v.begin(); lit != v.end(); lit++) {
			avg_lt += (*lit).dbl();
		}
		avg_lt /= v.size();
		//std:: cout << "    lifetime " << avg_lt << "s = " << avg_lt / 1000.0 << "ms" << std::endl;
		return (double) avg_lt;
	} else {
		return 0.0;
	}
} // avg_lifetime()

void Sink::determineQueueSizes() {

	std::string queue;
	char buffer[3];

	for (int i = 0; i < (_nofCoS+1); i++) {
		queue = "passiveQueue";
		sprintf(buffer, "%d", i);
		buffer[2] = '\0';
		queue += buffer;
		cModule *module = getParentModule()->getSubmodule(queue.c_str());
		IPassiveQueue *pqueue = dynamic_cast<IPassiveQueue *>(module);
		if (pqueue != NULL)
			std::cout << queue.c_str() << " length " << pqueue->length()
					<< std::endl;
	}
} // determineQueueSizes()

}
;
//namespace

