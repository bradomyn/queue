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
		std::cout << "server none" << std::endl;
		Useful::getInstance()->appendToFile("out.txt", "server none");
	} else if (strcmp(algName, "priority") == 0) {
		_scheduling = 1;
		std::cout << "server priority" << std::endl;
		Useful::getInstance()->appendToFile("out.txt", "server priority");
	} else if (strcmp(algName, "feedback") == 0) {
		_scheduling = 2;
		std::cout << "server feedback" << std::endl;
		Useful::getInstance()->appendToFile("out.txt", "server feedback");
	} else if (strcmp(algName, "original") == 0) {
		_scheduling = 3;
		std::cout << "server original" << std::endl;
		Useful::getInstance()->appendToFile("out.txt", "server original");
	} else if (strcmp(algName, "7first") == 0) {
		_scheduling = 4;
		std::cout << "server 7first" << std::endl;
		Useful::getInstance()->appendToFile("out.txt", "server 7first");
	} else if (strcmp(algName, "feedback2") == 0) {
		_scheduling = 5;
		std::cout << "server feedback2" << std::endl;
		Useful::getInstance()->appendToFile("out.txt", "server feedback2");
	}

	_serviceTime = par("serviceTime");

	_capacity = par("capacity");

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
				if( triggerServiceMsg->isScheduled() )
					cancelAndDelete(triggerServiceMsg);
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
			if (msg == triggerServiceMsg) {
				if (_order.size()>0 ) {
					map<simtime_t, Packet*>::iterator it;
					it = _order.begin();
					// oldest packet is first in map
					for( it = _order.begin(); it!=_order.end(); it++ ) {
						Packet * packet = it->second;
						if( packet->getPriority()==7 ) {
							//packet->setTimestamp();
							send(packet, "out");
							numSent++;
							//std::cout << "7 sent " << packet->getName() << " orders " << _order.size() << std::endl;
							_order.erase(packet->getCreationTime());
							//std::cout << " " << _order.size() << std::endl;
						} else {
							// sort for timestamps
							//packet->setTimestamp();
							send(packet, "out");
							numSent++;
							//std::cout << "x sent " << packet->getName() << " orders " << _order.size() << std::endl;
							_order.erase(packet->getCreationTime());
							//std::cout << " " << _order.size() << std::endl;
						}
					}
				}
				if( triggerServiceMsg->isScheduled() )
					cancelAndDelete(triggerServiceMsg);
			} else {
				if (strcmp(msg->getName(), "trigger") != 0) {
					// fill internal storage
					Packet *packetServiced = check_and_cast<Packet *>(msg);
					_order.insert(pair<simtime_t, Packet*>(packetServiced->getCreationTime(), new Packet(*packetServiced)));
					//std::cout << "inserted " << packetServiced->getName() << " orders " << _order.size() << std::endl;
					//packetServiced = NULL;
					//std::cout << "packetServiced: " << packetServiced->getName() << std::endl;

					scheduleAt(simTime() + _serviceTime, triggerServiceMsg);
				}
			}
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
				if( triggerServiceMsg->isScheduled() )
					cancelAndDelete(triggerServiceMsg);
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
		case 5:	// feedback 2
			// use with WRS1.ned
		if (msg == triggerServiceMsg) {
			// check internal queues
			vector<Packet*>::iterator it;

			// TODO
			simtime_t timeDist = simTime()-1;

			checkWaitingTimeAndCapacityAndMoveToOtherQueue(0, _iq0, _iq1, timeDist);
			checkWaitingTimeAndCapacityAndMoveToOtherQueue(1, _iq1, _iq2, timeDist);
			checkWaitingTimeAndCapacityAndMoveToOtherQueue(2, _iq2, _iq3, timeDist);
			checkWaitingTimeAndCapacityAndMoveToOtherQueue(3, _iq3, _iq4, timeDist);
			checkWaitingTimeAndCapacityAndMoveToOtherQueue(4, _iq4, _iq5, timeDist);
			checkWaitingTimeAndCapacityAndMoveToOtherQueue(5, _iq5, _iq6, timeDist);
			checkWaitingTimeAndCapacityAndMoveToOtherQueue(6, _iq6, _iq7, timeDist);

			if (_iq7.size() > 0) {
				it = _iq7.begin();
				while( it!=_iq7.end() ) {
					if( (simTime()-(*it)->getCreationTime())> timeDist ) {
						send(*it, "out");
						_iq7.erase(it);
					}
				}
			}
			if( triggerServiceMsg->isScheduled() )
				cancelAndDelete(triggerServiceMsg);
		} else {
			if (strcmp(msg->getName(), "trigger") != 0) {
				/*if (packetServiced) {
					std::cout << "packet arrived while already servicing one "
							<< packetServiced->getName() << " vs. "
							<< msg->getName() << std::endl;
					error("packet arrived while already servicing one");
				}*/
				Packet *packetServiced = check_and_cast<Packet *>(msg);
				scheduleAt(simTime() + _serviceTime, triggerServiceMsg);

				switch (packetServiced->getPriority()) {
				case 0:
					_iq0.push_back(packetServiced);
					break;
				case 1:
					_iq1.push_back(packetServiced);
					break;
				case 2:
					_iq2.push_back(packetServiced);
					break;
				case 3:
					_iq3.push_back(packetServiced);
					break;
				case 4:
					_iq4.push_back(packetServiced);
					break;
				case 5:
					_iq5.push_back(packetServiced);
					break;
				case 6:
					_iq6.push_back(packetServiced);
					break;
				case 7:
					_iq7.push_back(packetServiced);
					break;
				}
			}
		}
			break;
		default:
			break;
		}	// switch
		//} // else

		if (strcmp(msg->getName(), "trigger") == 0) {
			cancelAndDelete(msg);
			//std::cout << "trigger deleted" << std::endl;
		}

} // handleMessage()

int Server::determineQueueSize(vector<Packet*> v) {
	int queuesize = 0;
	vector<Packet*>::iterator it = v.begin();
	for( it = v.begin(); it!= v.end(); it++ ) {
		queuesize += (*it)->getSize();
	}

	return queuesize;
}

void Server::checkWaitingTimeAndCapacityAndMoveToOtherQueue(int priority, vector<Packet*> &v1, vector<Packet*> &v2, simtime_t timeDist) {
	if (v1.size() > 0) {
		vector<Packet*>::iterator it = v1.begin();
		int queuesize = determineQueueSize(v1);
		while (it != v1.end()) {
			if( ((simTime()-(*it)->getCreationTime()) > timeDist ) ||
			    ( queuesize>(_capacity-1500) ) ) {	// queue is almost full
				v2.push_back(*it); // move to higher queue
				//std::cout << "moved from " << priority << " to " << (priority+1) << ": " << (simTime()-(*it)->getCreationTime()) << std::endl;
				v1.erase(it);
				std::cout << ".";
				if( v1.size()>0 )
					it = v1.begin();
				else
					break;
			}
		}
		//std::cout << priority << " size: " << v1.size() << std::endl;
	}
}

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
	std::cout << " iq7: " << _iq7.size();
	std::cout << " iq6: " << _iq6.size() << " iq5: " << _iq5.size();
	std::cout << " iq4: " << _iq4.size() << " iq3: " << _iq3.size();
	std::cout << " iq2: " << _iq2.size() << " iq1: " << _iq1.size();
	std::cout << " iq0: " << _iq0.size() << std::endl;
	std::cout << " simTime() " << simTime() << " timeDist " << simTime()-1 << std::endl;
} // finish()

bool Server::isIdle()
{
    return packetServiced == NULL;
} // isIdle()

}; //namespace

