//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Source.h"

namespace queueing {


void SourceBase::initialize() {
	createdSignal = registerSignal("created");
	packetCounter = 0;
	WATCH(packetCounter);
	packetName = par("packetName").stringValue();
	if (packetName == "")
		packetName = getName();
}

Packet *SourceBase::createPacket() {
	char buf[80];
	sprintf(buf, "%.60s-%d", packetName.c_str(), ++packetCounter);
	Packet *packet = new Packet(buf);
	//packet->setKind(par("packetType"));
	int prio = Useful::getInstance()->generateRandomPriority();
	packet->setPriority(prio); //par("packetPriority"));
	return packet;
}

void SourceBase::finish() {
	emit(createdSignal, packetCounter);
}

//----

Define_Module(Source);

void Source::initialize() {
	SourceBase::initialize();
	startTime = par("startTime");
	stopTime = par("stopTime");
	numPackets = par("numPackets");

	// schedule the first message timer for start time
	scheduleAt(startTime, new cMessage("newPacketTimer"));

	string inputDataFile = par("inputDataFile").stringValue();
	_data = Useful::getInstance()->readDataList(inputDataFile);
	Useful::getInstance()->appendToFile("out.txt", inputDataFile.c_str());
	//Useful::getInstance()->appendToFile("source_out.txt", int(_data.size()));
	while(_data.size()< numPackets ) {
		std::vector<PacketDescription> v = Useful::getInstance()->readDataList(inputDataFile);
		_data.insert( _data.end(), v.begin(), v.end() );
	}
	//Useful::getInstance()->appendToFile("source_out.txt", int(_data.size()));

	//for( int i=0; i<_data.size(); i++ )
	  //std::cout << _data.at(i).getPriority() << " " << _data.at(i).getSize() << std::endl;


	WATCH(numCreated);
	numCreated = 0;
}

void Source::handleMessage(cMessage *msg) {
	ASSERT(msg->isSelfMessage());
#if 1
#if 1

	if( (numCreated < numPackets || numPackets < 0 || numPackets > numCreated || numCreated<_data.size() )
			&& (stopTime < 0 || stopTime > simTime()) ) {
		// reschedule the timer for the next message
		simtime_t sourceTime = simTime() + par("interArrivalTime").doubleValue();
		// TODO change inter arrival time to distribution
		//simtime_t sourceTime = simTime() + uniform(par("interArrivalTime").doubleValue(),1);
		scheduleAt(sourceTime, msg);
		//Useful::getInstance()->appendToFile("source_out.txt", numCreated);
		Packet *packet = generatePacket( _data.at(numCreated).getPriority(), _data.at(numCreated).getSize() );
		send(packet, "out");

		// trigger the server
		cMessage* trigger = new cMessage("trigger");
		send(trigger, "outTrigger");

		//std::cout << "source sent " << packet->getName() << ", trigger " << std::endl;

		//std::cout << "Message sent at " << sourceTime << std::endl;
		numCreated++;
	} else {
		std::cout << "No more packets available." << std::endl;
		// finished
		delete msg;
	}
#else
	if ((numPackets < 0 || numPackets > packetCounter)
			&& (stopTime < 0 || stopTime > simTime())) {
		// reschedule the timer for the next message
		simtime_t sourceTime = simTime() + par("interArrivalTime").doubleValue();
		scheduleAt(sourceTime, msg);

		Packet *packet = generatePacket(); //createPacket();
		send(packet, "out");
		//std::cout << "Message sent at " << sourceTime << std::endl;

		numCreated++;
	} else {
		// finished
		delete msg;
	}
#endif
#endif
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
	sprintf(name, "id: %ld, priority: %d; > %lf", packet->getId(), randomP, creationTime.dbl());
	name[79] = '\0';
	packet->setName(name);
	//std::cout << "packet (id: " << packet->getId() << ") priority set to: " << randomP << " size " << randomS << std::endl;

	packet->setTimestamp(creationTime);

	return packet;
} // generatePacket()

Packet * Source::generatePacket( int priority, int size ) {
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
	sprintf(name, "id: %ld, priority: %d; > %lf", packet->getId(), priority, creationTime.dbl());
	name[79] = '\0';
	packet->setName(name);
	//std::cout << "packet (id: " << packet->getId() << ") priority set to: " << priority << " size " << size << std::endl;

	packet->setTimestamp(creationTime);

	return packet;
} // generatePacket()

};
//namespace

