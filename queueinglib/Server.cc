//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Server.h"
#include "Packet.h"
#include "SelectionStrategiesServer.h"

namespace queueing {

Define_Module(Server);

Server::Server()
{
    selectionStrategy = NULL;
    packetServiced = NULL;
    endServiceMsg = NULL;

    triggerServiceMsg = NULL;
}

Server::~Server()
{
    delete selectionStrategy;
    delete packetServiced;
    cancelAndDelete(endServiceMsg);

    cancelAndDelete(triggerServiceMsg);
}

void Server::initialize()
{
    busySignal = registerSignal("busy");
    emit(busySignal, 0);

    endServiceMsg = new cMessage("end-service");

    packetServiced = NULL;

    selectionStrategy = SelectionStrategyServer::create(par("fetchingAlgorithm"), this, true);
    if (!selectionStrategy)
        error("invalid selection strategy");


    // keep a pointer to queue 7
    _q7 = getQueue(7);

    // pointers to other queues
    for( int i=6; i>-1; i-- )
    	_qs.push_back( getQueue(i) );

    triggerServiceMsg = new cMessage("triggerServiceMessage");
    //triggerServiceMsg = new cMessage("trigger");

    WATCH(numSent);
    numSent = 0;

	WATCH(triggerCounter);
	triggerCounter = 0;

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

	_serviceTime = par("serviceTime");

} // initialize()

void Server::serveCurrentPacket() {
	if( packetServiced!=NULL ) {
		simtime_t d = simTime() - triggerServiceMsg->getSendingTime();
		packetServiced->setTotalServiceTime(packetServiced->getTotalServiceTime() + d);
		send(packetServiced, "out");
		numSent++;
		//std::cout << "server sent " << packetServiced->getName() << std::endl;
		packetServiced = NULL;
		emit(busySignal, 0);
	}
} // serveCurrentPacket()()

void Server::handleMessage(cMessage *msg)
{
	//std::cout << "server received " << msg->getName() << std::endl;
	Packet *packet;
	int k;

		switch(_scheduling) {
		case 0:	// none
#if 0
			// use with WRS.ned
			// send through without thinking
			packet = check_and_cast<Packet *>(msg);
			packet->setTimestamp();
			send(packet, "out");
			numSent++;
#else
			if (msg == triggerServiceMsg) {
				serveCurrentPacket();
			} else {
				if (strcmp(msg->getName(), "trigger") != 0) {
					if (packetServiced) {
						std::cout << "packet arrived while already servicing one "
								<< packetServiced->getName() << " vs. "
								<< msg->getName() << std::endl;
						error("packet arrived while already servicing one");
						//serveCurrentPacket();
					}
					packetServiced = check_and_cast<Packet *>(msg);
					scheduleAt(simTime() + _serviceTime, triggerServiceMsg);
				}
			}
#endif
			break;
		case 1: // priority
		// use with WRS1.ned
		//std::cout << this->getName() << " priority" << std::endl;
		if (msg == triggerServiceMsg) {
			//std::cout << " triggerServiceMsg: ";
			serveCurrentPacket();
		} else {
			if (strcmp(msg->getName(), "trigger") == 0) {

				// trigger test
				//std::cout << " server triggered ";

				//TODO service time, currently service time is trigger intervall (not nice)
				// check queue lengths and request if length>0 as long as length>0
				if (_q7->length() > 0) {
					std::cout << "q7 length " << _q7->length();
					while (_q7->length() > 0) {
						//std::cout << "request 7" << std::endl;
						_q7->request(0);
					}
				} else {
					// check all other queue gates
					for (int i = 6; i > -1; i--) {
						if (_qs.at(i)->length() > 0) {
							//std::cout << "q" << i << " length " << _qs.at(i)->length();
							//while( _qs.at(i)->length()>0 ) {
							if (_qs.at(i)->length()) {
								//std::cout << " request i " << i << std::endl;
								_qs.at(i)->request(0);
							}
						}
					}
				}
			} else {
				if (packetServiced) {
					std::cout << "packet arrived while already servicing one "
							<< packetServiced->getName() << " vs. "
							<< msg->getName() << std::endl;
					error("packet arrived while already servicing one");
				}

				packetServiced = check_and_cast<Packet *>(msg);
				//std::cout << "packetServiced: " << packetServiced->getName()
					//	<< std::endl;
				scheduleAt(simTime() + _serviceTime, triggerServiceMsg);
			}
		}
			break;
		case 2: // feedback
			// std::cout << this->getName() << " feedback" << std::endl;
			// TODO
			// store information internally how long a message is already there
			// serve oldest first
			break;
		case 3: // original
			//std::cout << this->getName() << " original" << std::endl;
			// use with WRS.ned
		    if (msg==endServiceMsg) {
		        ASSERT(packetServiced!=NULL);
		        simtime_t d = simTime() - endServiceMsg->getSendingTime();
		        packetServiced->setTotalServiceTime(packetServiced->getTotalServiceTime() + d);
		        send(packetServiced, "out");
		        numSent++;
		        packetServiced = NULL;
		        emit(busySignal, 0);

		        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

		        // examine all input queues, and request a new packet from a non empty queue
		        k = selectionStrategy->select();
		        if (k >= 0)
		        {
		            EV << "requesting packet from queue " << k << endl;
		            cGate *gate = selectionStrategy->selectableGate(k);
		            check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
		        }
		    } else {
		        if (packetServiced)
		            error("packet arrived while already servicing one");

		        packetServiced = check_and_cast<Packet *>(msg);
		        scheduleAt(simTime()+_serviceTime, endServiceMsg);
		        emit(busySignal, 1);

		        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"cyan");
		    }
			break;
		case 4:	// 7first
			// use with WRS1.ned
			//std::cout << this->getName() << " 7first" << std::endl;
			if (msg == triggerServiceMsg) {
				//std::cout << " triggerServiceMsg: ";
				serveCurrentPacket();
			} else {
				if (strcmp(msg->getName(), "trigger") == 0) {
					// trigger test
					//std::cout << " server triggered ";
					// check all other queue gates
					for (int i = 6; i > -1; i--) {
						if (_qs.at(i)->length() > 0) {
							//std::cout << "q" << i << " length " << _qs.at(i)->length();
							while( _qs.at(i)->length()>0 ) {
							//if (_qs.at(i)->length()) {
								//std::cout << " request i " << i << std::endl;
								_qs.at(i)->request(0);
							}
						}
					}
				} else {
					if (packetServiced) {
						std::cout << "packet arrived while already servicing one "
								<< packetServiced->getName() << " vs. "
								<< msg->getName() << std::endl;
						error("packet arrived while already servicing one");
						//serveCurrentPacket();
					}

					packetServiced = check_and_cast<Packet *>(msg);
					//std::cout << "packetServiced: " << packetServiced->getName()
						//	<< std::endl;
					scheduleAt(simTime() + _serviceTime, triggerServiceMsg);
				}
			}
			break;
		default:
			break;
		}	// switch
		//} // else
} // handleMessage()

IPassiveQueue *Server::getQueue(int index) {
	std::string queue = "passiveQueue";
	char buffer[3];

	sprintf(buffer,"%d",index);
	buffer[2]='\0';
	queue += buffer;
	cModule *module = getParentModule()->getSubmodule(queue.c_str());
	IPassiveQueue *pqueue = dynamic_cast<IPassiveQueue *>(module);

	return pqueue;
} // getQueue()

void Server::finish()
{
} // finish()

bool Server::isIdle()
{
    return packetServiced == NULL;
} // isIdle()

}; //namespace

