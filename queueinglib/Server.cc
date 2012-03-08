//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Server.h"
#include "Job.h"
#include "SelectionStrategiesServer.h"

namespace queueing {

Define_Module(Server);

Server::Server()
{
    selectionStrategy = NULL;
    jobServiced = NULL;
    endServiceMsg = NULL;

    triggerServiceMsg = NULL;
}

Server::~Server()
{
    delete selectionStrategy;
    delete jobServiced;
    cancelAndDelete(endServiceMsg);

    cancelAndDelete(triggerServiceMsg);
}

void Server::initialize()
{
    busySignal = registerSignal("busy");
    emit(busySignal, 0);

    endServiceMsg = new cMessage("end-service");

    jobServiced = NULL;

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
	}

} // initialize()

void Server::serveCurrentJob() {
	if( jobServiced!=NULL ) {
		simtime_t d = simTime() - triggerServiceMsg->getSendingTime();
		jobServiced->setTotalServiceTime(jobServiced->getTotalServiceTime() + d);
		send(jobServiced, "out");
		numSent++;
		//std::cout << "server sent " << jobServiced->getName() << std::endl;
		jobServiced = NULL;
		emit(busySignal, 0);
	}
} // serveCurrentJob()()

void Server::handleMessage(cMessage *msg)
{
	//std::cout << "server received " << msg->getName() << std::endl;
	Job *job;
	int k;
		simtime_t serviceTime = par("serviceTime");
		switch(_scheduling) {
		case 0:	// none
			// use with WRS.ned
			// send through without thinking
			job = check_and_cast<Job *>(msg);
			job->setTimestamp();
			send(job, "out");
			numSent++;
			break;
		case 1: // priority
		// use with WRS1.ned
		//std::cout << this->getName() << " priority" << std::endl;
		if (msg == triggerServiceMsg) {
			//std::cout << " triggerServiceMsg: ";
			serveCurrentJob();
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
				if (jobServiced) {
					std::cout << "job arrived while already servicing one "
							<< jobServiced->getName() << " vs. "
							<< msg->getName() << std::endl;
					error("job arrived while already servicing one");
					//serveCurrentJob();
				}

				jobServiced = check_and_cast<Job *>(msg);
				//std::cout << "jobServiced: " << jobServiced->getName()
					//	<< std::endl;
				simtime_t serviceTime = par("serviceTime");
				scheduleAt(simTime() + serviceTime, triggerServiceMsg);
			}
		}
			break;
		case 2: // feedback
			// TODO
			break;
		case 3: // original
			//std::cout << this->getName() << " original" << std::endl;
			// use with WRS.ned
		    if (msg==endServiceMsg) {
		        ASSERT(jobServiced!=NULL);
		        simtime_t d = simTime() - endServiceMsg->getSendingTime();
		        jobServiced->setTotalServiceTime(jobServiced->getTotalServiceTime() + d);
		        send(jobServiced, "out");
		        numSent++;
		        jobServiced = NULL;
		        emit(busySignal, 0);

		        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

		        // examine all input queues, and request a new job from a non empty queue
		        k = selectionStrategy->select();
		        if (k >= 0)
		        {
		            EV << "requesting job from queue " << k << endl;
		            cGate *gate = selectionStrategy->selectableGate(k);
		            check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
		        }
		    } else {
		        if (jobServiced)
		            error("job arrived while already servicing one");

		        jobServiced = check_and_cast<Job *>(msg);
		        simtime_t serviceTime = par("serviceTime");
		        scheduleAt(simTime()+serviceTime, endServiceMsg);
		        emit(busySignal, 1);

		        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"cyan");
		    }
			//std::cout << "original: " << msg->getName() << std::endl;
			// Original
/*			if (msg == endServiceMsg) {
					//std::cout << "endServiceMsg " << endServiceMsg->getName() << std::endl;
					ASSERT(jobServiced!=NULL);
					simtime_t d = simTime() - endServiceMsg->getSendingTime();
					jobServiced->setTotalServiceTime(
							jobServiced->getTotalServiceTime() + d);
					send(jobServiced, "out");
					std::cout << jobServiced->getName() << " with prio "
							<< jobServiced->getPriority() << " sent" << std::endl;
					numSent++;
					jobServiced = NULL;
					emit(busySignal, 0);

					if (ev.isGUI())
						getDisplayString().setTagArg("i", 1, "");

					// examine all input queues, and request a new job from a non empty queue
					int k = selectionStrategy->select();
					std::cout << "server selected gate " << k << std::endl;
					if (k >= 0) {
						EV << "requesting job from queue " << k << endl;
						std::cout << "requesting job from queue " << k << std::endl;
						cGate *gate = selectionStrategy->selectableGate(k);
						check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(
								gate->getIndex());
					}
				} else {
					if (jobServiced) {
						error("job arrived while already servicing one");
						std::cout << "job arrived while already servicing one"
								<< std::endl;
					}

					if (strcmp(msg->getName(), triggerServiceMsg->getName()) != 0) {
						jobServiced = check_and_cast<Job *>(msg);
						std::cout << "jobServiced " << jobServiced->getName()
								<< " with prio " << jobServiced->getPriority()
								<< " sent" << std::endl;
						simtime_t serviceTime = par("serviceTime");
						scheduleAt(simTime() + serviceTime, endServiceMsg);
						emit(busySignal, 1);
						if (ev.isGUI())
							getDisplayString().setTagArg("i", 1, "cyan");
					}
			}*/
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
}

void Server::finish()
{
}

bool Server::isIdle()
{
    return jobServiced == NULL;
}

}; //namespace

