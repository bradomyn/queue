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
#include "Server.h"
#include "Source.h"
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

    lifeTimeSignal0 = registerSignal("lifeTime0");
    lifeTimeSignal1 = registerSignal("lifeTime1");
    lifeTimeSignal2 = registerSignal("lifeTime2");
    lifeTimeSignal3 = registerSignal("lifeTime3");
    lifeTimeSignal4 = registerSignal("lifeTime4");
    lifeTimeSignal5 = registerSignal("lifeTime5");
    lifeTimeSignal6 = registerSignal("lifeTime6");
    lifeTimeSignal7 = registerSignal("lifeTime7");

    numReceived = 0;
    WATCH(numReceived);
}

void Sink::handleMessage(cMessage *msg)
{
	if( strcmp(msg->getName(),"trigger")!=0 ){
		simtime_t currentTime = simTime();
		simtime_t lifetime = currentTime-msg->getCreationTime();
		Packet *packet = check_and_cast<Packet *>(msg);

		numReceived++;

		//std::cout << "#" << numReceived << ": " << packet->getName() << " received. ct " << msg->getCreationTime() << " st " << currentTime << " lifetime " << lifetime  << std::endl;

		// investigate distribution of packet sizes (payload)
		int size = packet->getSize();
		if( size < 500 ) {
			v00.push_back(0);
		} else if ( size>=500 && size <1000 ) {
			v01.push_back(1);
		} else if( size >=1000 && size <=1500 ) {
			v02.push_back(2);
		}

		switch( packet->getPriority() ) {
		case 0:
			v0.push_back(lifetime);
			v00q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal0, simTime()- packet->getCreationTime());
			break;
		case 1:
			v1.push_back(lifetime);
			v01q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal1, simTime()- packet->getCreationTime());
			break;
		case 2:
			v2.push_back(lifetime);
			v02q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal2, simTime()- packet->getCreationTime());
			break;
		case 3:
			v3.push_back(lifetime);
			v03q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal3, simTime()- packet->getCreationTime());
			break;
		case 4:
			v4.push_back(lifetime);
			v04q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal4, simTime()- packet->getCreationTime());
			break;
		case 5:
			v5.push_back(lifetime);
			v05q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal5, simTime()- packet->getCreationTime());
			break;
		case 6:
			v6.push_back(lifetime);
			v06q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal6, simTime()- packet->getCreationTime());
			break;
		case 7:
			v7.push_back(lifetime);
			v07q.push_back(packet->getOperationCounter() * 0.000000008);
			emit(lifeTimeSignal7, simTime()- packet->getCreationTime());
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
	}

    if (!keepPackets) {
    	if( msg->isScheduled() )
    		cancelAndDelete(msg);
    	delete msg;
    }

}

double Sink::avg_lifetime(vector<double> v) {
	double avg_lt = 0.;
	std::vector<double>::iterator lit;
	for( lit = v.begin(); lit!=v.end(); lit++ ) {
		avg_lt += (*lit);
	}
	avg_lt /= v.size();
	//std:: cout << "    lifetime " << avg_lt << "s = " << avg_lt / 1000.0 << "ms" << std::endl;
	return (double)avg_lt;
}

double Sink::avg_lifetime(vector<simtime_t> v) {
	double avg_lt = 0.;
	std::vector<simtime_t>::iterator lit;
	for( lit = v.begin(); lit!=v.end(); lit++ ) {
		avg_lt += (*lit).dbl();
	}
	avg_lt /= v.size();
	//std:: cout << "    lifetime " << avg_lt << "s = " << avg_lt / 1000.0 << "ms" << std::endl;
	return (double)avg_lt;
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

	std::cout << "packet sizes " << v00.size() << " " << v01.size() << " " << v02.size();

	// find out number of objects still stuck in server
	cModule *server = (cModule*)getParentModule()->findObject("server", true);
	int nofObjects = (check_and_cast<Server *>(server))->getLiveObjectCount();
	cout << " " << nofObjects << std::endl;
	int nofDropped = (check_and_cast<Server *>(server))->getDropped().size();
	cout << "dropped " << nofDropped;

	cModule *source = (cModule*)getParentModule()->findObject("source", true);
	int nofCreated = (check_and_cast<Source *>(source))->getCreated();
	int nofArrived = v0.size()+v1.size()+v2.size()+v3.size()+v4.size()+v5.size()+v6.size()+v7.size();
	cout << " created: " << nofCreated  << " arrived: " << nofArrived << endl;

	// output to a file
	string str;
	char buf[50];
	sprintf(buf,"p 0: avg %lf size %d", avg_lifetime(v0), v0.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	sprintf(buf, "p 1: avg %lf size %d", avg_lifetime(v1), v1.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	sprintf(buf, "p 2: avg %lf size %d", avg_lifetime(v2), v2.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	sprintf(buf, "p 3: avg %lf size %d", avg_lifetime(v3), v3.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	sprintf(buf, "p 4: avg %lf size %d", avg_lifetime(v4), v4.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	sprintf(buf, "p 5: avg %lf size %d", avg_lifetime(v5), v5.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	sprintf(buf, "p 6: avg %lf size %d", avg_lifetime(v6), v6.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	sprintf(buf, "p 7: avg %lf size %d", avg_lifetime(v7), v7.size());
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);

	sprintf(buf, "dropped: %d created: %d arrived: %d", nofDropped, nofCreated, nofArrived );
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);

	/*vector<int>::iterator it;
	double avgOps = 0.;
	for( it=(check_and_cast<Server *>(server))->getOps().begin(); it!=(check_and_cast<Server *>(server))->getOps().end(); it++ ) {
		//cout << (*it) << "  ";
		//125000000 Operationen pro Sekunde moeglich im WRS
		//1/125000000 = 0.000000008
		//              0.0000000125
		avgOps += (0.000000008*(*it));
		sprintf(buf, "%d %lf", *it, (0.000000008*(*it)) );
		str = string(buf);
		Useful::getInstance()->appendToFile("ops.txt", str);
	}
	avgOps /= (check_and_cast<Server *>(server))->getOps().size();
	cout << (check_and_cast<Server *>(server))->getOps().size() << " avg " << avgOps << endl;*/

	// evaluate operation counter from packet
	double avg = avg_lifetime(v00q);
	cout << /*"s 0: " <<*/ avg << endl;
	sprintf(buf, "op 0: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	avg = avg_lifetime(v01q);
	cout << /*"s 1: " <<*/ avg << endl;
	sprintf(buf, "op 1: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	avg = avg_lifetime(v02q);
	cout << /*"s 2: " <<*/ avg << endl;
	sprintf(buf, "op 2: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	avg = avg_lifetime(v03q);
	cout << /*"s 3: " <<*/ avg << endl;
	sprintf(buf, "op 3: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	avg = avg_lifetime(v04q);
	cout << /*"s 4: " <<*/ avg << endl;
	sprintf(buf, "op 4: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	avg = avg_lifetime(v05q);
	cout << /*"s 5: " <<*/ avg << endl;
	sprintf(buf, "op 5: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	avg = avg_lifetime(v06q);
	cout << /*"s 6: " <<*/ avg << endl;
	sprintf(buf, "op 6: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	avg = avg_lifetime(v07q);
	cout << /*"s 7: " <<*/ avg << endl;
	sprintf(buf, "op 7: %lf", avg);
	str = string(buf);
	Useful::getInstance()->appendToFile("out.txt", str);
	// 4e-08 = 4*10^-8 	 	= 0,00000004
	// 4.8e-08 = 4.8*10^-8 	= 0,000000048

	// remove all undisposed messages in the end!
	this->setPerformFinalGC(true);
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

