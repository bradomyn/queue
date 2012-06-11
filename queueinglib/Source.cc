//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Source.h"
#include "Packet.h"

namespace queueing {

void SourceBase::initialize() {
	createdSignal = registerSignal("created");
	jobCounter = 0;
	WATCH(jobCounter);
	packetName = par("jobName").stringValue();
	if (packetName == "")
		packetName = getName();
}

Packet *SourceBase::createJob() {
	char buf[80];
	sprintf(buf, "%.60s-%d", packetName.c_str(), ++packetCounter);
	Packet *packet = new Packet(buf);
	//packet->setKind(par("packetType"));
	int prio = Useful::getInstance()->generateRandomPriority();
	packet->setPriority(prio); //par("packetPriority"));
	return packet;
}

void SourceBase::finish() {
	// remove all undisposed messages in the end!
	this->setPerformFinalGC(true);
	emit(createdSignal, jobCounter);
}

//----

Define_Module(Source);

void Source::initialize() {
	SourceBase::initialize();
	startTime = par("startTime");
	stopTime = par("stopTime");
	numPackets = par("numPackets");

	// schedule the first message timer for start time
	//scheduleAt(startTime, new cMessage("newJobTimer"));

	// schedule the first message timer for start time
	scheduleAt(startTime, new cMessage("newPacketTimer"));

	_inputDataFile = par("inputDataFile").stringValue();
	_data = Useful::getInstance()->readDataList(_inputDataFile);
	//Useful::getInstance()->appendToFile("out.txt", _inputDataFile.c_str());
	//Useful::getInstance()->appendToFile("source_out.txt", int(_data.size()));
	while (_data.size() < numPackets) {
		std::vector<PacketDescription> v = Useful::getInstance()->readDataList(
				_inputDataFile);
		_data.insert(_data.end(), v.begin(), v.end());
	}
	//Useful::getInstance()->appendToFile("source_out.txt", int(_data.size()));

	//for( int i=0; i<_data.size(); i++ )
	//std::cout << "i: " << i << " " << _data.at(i).getPriority() << " " << _data.at(i).getSize() << std::endl;

	WATCH(numCreated);
	numCreated = 0;

	cModule *module = getParentModule()->getSubmodule("sink");
	_sink =  dynamic_cast<Sink*>(module);

	for( int i=0; i<8; i++ ) {
		_sent.push_back(0);
	}

} // initialize()

void Source::handleMessage(cMessage *msg) {
	ASSERT(msg->isSelfMessage());
	//cout << __FILE__ << ": " << __FUNCTION__ << endl;
	int numReceived = _sink->getNumReceived();
	//cout << "numPackets: " << numPackets << " numCreated " << numCreated << " numReceived: " << numReceived << endl;

	if ( numReceived != numPackets ) {
		simtime_t sourceTime = simTime()
				+ par("interArrivalTime").doubleValue();
		scheduleAt(sourceTime, msg);

		// trigger the server
		cMessage* trigger = new cMessage("trigger");
		send(trigger, "outTrigger");

		if ((numCreated < numPackets || numPackets < 0
				|| numPackets > numCreated )) {
			// reschedule the timer for the next message
			//Useful::getInstance()->appendToFile("source_out.txt", numCreated);
			int i = 0;
			vector<PacketDescription>::iterator it;

			if (_data.size() > 8) {
				for (it = _data.begin(), i = 0; it != _data.end(); it++, i++) {
					if (i < 8) {
						//cout << "huhu " << (*it).getPriority() << " " << (*it).getSize() << endl;
						Packet *p = generatePacket((*it).getPriority(), (*it).getSize());
						send2Queue(p);
						_data.erase(it);
					}
					if (i == 8)
						break;
				}
			}
		}
	}
}

void Source::send2Queue(Packet* packet) {
	numCreated++;
	switch (packet->getPriority()) {
	case 0:
		send(packet, "out", 0);
		_sent.at(0) = _sent.at(0)+1;
		break;
	case 1:
		send(packet, "out", 1);
		_sent.at(1) = _sent.at(1)+1;
		break;
	case 2:
		send(packet, "out", 2);
		_sent.at(2) = _sent.at(2)+1;
		break;
	case 3:
		send(packet, "out", 3);
		_sent.at(3) = _sent.at(3)+1;
		break;
	case 4:
		send(packet, "out", 4);
		_sent.at(4) = _sent.at(4)+1;
		break;
	case 5:
		send(packet, "out", 5);
		_sent.at(5) = _sent.at(5)+1;
		break;
	case 6:
		send(packet, "out", 6);
		_sent.at(6) = _sent.at(6)+1;
		break;
	case 7:
		send(packet, "out", 7);
		_sent.at(7) = _sent.at(7)+1;
		break;
	}
}

Packet * Source::generatePacket() {
	//log("test");
	char buf[80];
	std::string packetName = "j";
	sprintf(buf, "%.60s-%d", packetName.c_str(), ++packetCounter);
	Packet *packet = new Packet(buf);
	int randomP = Useful::getInstance()->generateRandomPriority();

	// TODO work with a fixed, repeatable data set
	packet->setPriority(randomP);

	int randomS = Useful::getInstance()->generateRandomSize();

	// TODO work with a fixed, repeatable data set
	packet->setSize(randomS);

	// to build a data file:
	//Useful::getInstance()->writeRandomDataToList("data.txt", randomP, randomS);

	simtime_t creationTime = simTime();
	char name[80];
	//sprintf(name, "id: %ld, priority: %d; %f", packet->getId(), random, triggerTime);
	sprintf(name, "id: %ld, priority: %d; > %lf", packet->getId(), randomP,
			creationTime.dbl());
	name[79] = '\0';
	packet->setName(name);
	//std::cout << "packet (id: " << packet->getId() << ") priority set to: " << randomP << " size " << randomS << std::endl;

	packet->setTimestamp(creationTime);

	return packet;
} // generatePacket()

Packet * Source::generatePacket(int priority, int size) {
	//log("test");
	char buf[80];
	std::string packetName = "j";
	sprintf(buf, "%.60s-%d", packetName.c_str(), ++packetCounter);

	Packet *packet = new Packet(buf);
	packet->setPriority(priority);
	packet->setSize(size);

	simtime_t creationTime = simTime();
	char name[80];
	//sprintf(name, "id: %ld, priority: %d; %f", packet->getId(), random, triggerTime);
	sprintf(name, "id: %ld, priority: %d; > %lf", packet->getId(), priority,
			creationTime.dbl());
	name[79] = '\0';
	packet->setName(name);
	//std::cout << "packet (id: " << packet->getId() << ") priority set to: " << priority << " size " << size << std::endl;

	packet->setTimestamp(creationTime);

	return packet;
} // generatePacket()

//----

Define_Module(SourceOnce);

void SourceOnce::initialize() {
	SourceBase::initialize();
	simtime_t time = par("time");
	scheduleAt(time, new cMessage("newJobTimer"));
}

void SourceOnce::handleMessage(cMessage *msg) {
	ASSERT(msg->isSelfMessage());
	delete msg;

	int n = par("numJobs");
	for (int i = 0; i < n; i++) {
		Packet *job = createJob();
		send(job, "out");
	}
}

}
;
//namespace

