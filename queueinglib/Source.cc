//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Source.h"
#include "Packet_m.h"

namespace queueing {

void SourceBase::initialize() {
	createdSignal = registerSignal("created");
	jobCounter = 0;
	WATCH(jobCounter);
	packetName = par("jobName").stringValue();
	if (packetName == "")
		packetName = getName();
}

WRPacket *SourceBase::createJob() {
	char buf[80];
	sprintf(buf, "%.60s-%d", packetName.c_str(), ++packetCounter);
	WRPacket *packet = new WRPacket(buf);
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

	_nofCoS = par ("nofCoS");

	// schedule the first message timer for start time
	//scheduleAt(startTime, new cMessage("newJobTimer"));

	// schedule the first message timer for start time
	scheduleAt(startTime, new cMessage("newPacketTimer"));

	startSendingPacket = new cMessage("startSendingPacket");

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

	cModule *module1 = getParentModule()->getSubmodule("sink1");
	_sink1 =  dynamic_cast<Sink*>(module1);

	for( int i=0; i<_nofCoS; i++ ) {
		_sent.push_back(0);
	}

	for( int i=_nofCoS-1; i>-1; i-- ) {
		outputgates.push_back(getGate(i));
		channels.push_back(check_and_cast<cDatarateChannel *> (getGate(i)->getTransmissionChannel()));
	}
} // initialize()

void Source::handleMessage(cMessage *msg) {
	ASSERT(msg->isSelfMessage());
	//cout << __FILE__ << ": " << __FUNCTION__ << endl;
	int numReceived = _sink->getNumReceived();
	int numReceived1 = _sink1->getNumReceived();
	//cout << "numPackets: " << numPackets << " numCreated " << numCreated << " numReceived: " << numReceived << endl;

#if 1
	// 1000000000 bits per second = 1000 bits per us
	if(msg = startSendingPacket ) {
		WRPacket *p = check_and_cast<WRPacket*>(msg);
		send2Queue(p);
	} else {
		if ( (numReceived != numPackets) && (numReceived1!=numPackets)) {

		if ((numCreated < numPackets || numPackets < 0
						|| numPackets > numCreated )) {
			vector<PacketDescription>::iterator it;

			if (_data.size() > _nofCoS) {
				it = _data.begin();
				while( it!=_data.end() ) {
					//cout << "huhu " << i << " " << (*it).getPriority() << " " << (*it).getSize() << endl;
					WRPacket *p = generatePacket((*it).getPriority(), (*it).getSize());
					p->setTimestamp(simTime());
					//cout << "bitl: " << p->getBitLength() << " bytes: " << p->getByteLength() << endl;
					//send2Queue(p);
					//_data.erase(it);
					//it = _data.begin();
					//switch prio, determine datarate, schedule message
					channels[0]->getTransmissionFinishTime();
					scheduleAt(p, startSendingPacket);
					}
				}
			}
		}
	}
#else
	if ( (numReceived != numPackets) && (numReceived1!=numPackets)) {
		simtime_t sourceTime = simTime()
				+ par("interArrivalTime").doubleValue();
		scheduleAt(sourceTime, msg);

		// trigger the server
		//cMessage* trigger = new cMessage("trigger");
		//send(trigger, "outTrigger");

		if ((numCreated < numPackets || numPackets < 0
				|| numPackets > numCreated )) {
			// reschedule the timer for the next message
			//Useful::getInstance()->appendToFile("source_out.txt", numCreated);
			int i = 0;
			vector<PacketDescription>::iterator it;

			if (_data.size() > _nofCoS) {
				//for (it = _data.begin(), i = 0; it != _data.end(); it++, i++)
				it = _data.begin();
				i=0;
				while( it!=_data.end() ) {
					if (i < _nofCoS) {
						//cout << "huhu " << i << " " << (*it).getPriority() << " " << (*it).getSize() << endl;
						WRPacket *p = generatePacket((*it).getPriority(), (*it).getSize());
						p->setTimestamp(simTime());
						cout << "bitl: " << p->getBitLength() << " bytes: " << p->getByteLength() << endl;
						send2Queue(p);
						_data.erase(it);
						it = _data.begin();
					}
					if (i == _nofCoS) {
						i=0;
						break;
					}
					i++;
				}
			}
		}
	}
#endif
}

void Source::send2Queue(WRPacket* packet) {
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

WRPacket * Source::generatePacket() {
	//log("test");
	char buf[80];
	std::string packetName = "j";
	sprintf(buf, "%.60s-%d", packetName.c_str(), ++packetCounter);
	WRPacket *packet = new WRPacket(buf);
	int randomP = Useful::getInstance()->generateRandomPriority();

	// TODO work with a fixed, repeatable data set
	packet->setPriority(randomP);

	int randomS = Useful::getInstance()->generateRandomSize();
	packet->setByteLength(randomS);

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

WRPacket * Source::generatePacket(int priority, int size) {
	//log("test");
	char buf[80];
	std::string packetName = "j";
	sprintf(buf, "%.60s-%d", packetName.c_str(), ++packetCounter);

	WRPacket *packet = new WRPacket(buf);
	packet->setPriority(priority);
	packet->setByteLength(size);

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


cGate * Source::getGate(int index) {
	cGate *outputgate = gate("pppg", index);
	return outputgate;
} // getGate()

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
		WRPacket *job = createJob();
		send(job, "out");
	}
}

}
;
//namespace

