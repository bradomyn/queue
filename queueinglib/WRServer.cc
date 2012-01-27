//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

// SMa, 11.01.2012

#include "WRServer.h"
#include "Job.h"
#include "SelectionStrategies.h"
#include "IPassiveQueue.h"
#include "WRPassiveQueue.h"

namespace queueing {

Define_Module(WRServer);

WRServer::WRServer()
{
    selectionStrategy = NULL;
    jobServiced = NULL;
    endServiceMsg = NULL;
}

WRServer::~WRServer()
{
    delete selectionStrategy;
    delete jobServiced;
    cancelAndDelete(endServiceMsg);
}

void WRServer::initialize()
{
    busySignal = registerSignal("busy");
    emit(busySignal, 0);

    endServiceMsg = new cMessage("end-service");
    jobServiced = NULL;
    selectionStrategy = SelectionStrategy::create(par("fetchingAlgorithm"), this, true);

    if (!selectionStrategy) {
        error("invalid selection strategy");
    } else {
    	std::cout << "selection strategy " << selectionStrategy->getName() << std::endl;
    }
}

void WRServer::handleMessage(cMessage *msg)
{
	//std::cout << __FILE__ << ": " << __FUNCTION__ << ": l " << __LINE__;

#if 0
	// examine all input queues, and request a new job from a non empty queue
	/*int k = selectionStrategy->select();
	std::cout << "selection strategy " << k << std::endl;
	if (k >= 0) {
		EV << "requesting job from queue " << k << endl;
		std::cout << "requesting job from queue " << k << endl;
		cGate *gate = selectionStrategy->selectableGate(k);
		check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
	}*/

	//jobs.push_back(msg);

    //std::cout << "job serviced available: " << jobServiced << std::endl;
    if( jobServiced!=NULL ) {
		send(jobServiced, "out");	// faster

		// sending this direct takes more time (around 0.002 more)!
		/*cMessage *msgs = check_and_cast<cMessage*>(jobServiced);
    	cModule *targetModule = getParentModule()->getSubmodule("wrRouter");
    	sendDirect(msgs, targetModule, "sendDirect");*/
   		//std::cout << "sent to wrRouter" << std::endl;
		jobServiced = NULL;
		emit(busySignal, 0);


		// check for packets with highest priority
		WRPassiveQueue *q7 = check_and_cast<WRPassiveQueue *>( getParentModule()->findObject("wrQueue7", true) );
		if( q7->length()>0 ) {
			q7->request(0);
		} else {
			// check other queues
			std::string module;
			char buffer[3];
			Job *job = check_and_cast<Job *>(msg);
			int priority = job->getPriority();
			for( int i=6; i>-1; i-- ) {
				module = "wrQueue";
				sprintf(buffer,"%d", priority);
				buffer[2]='\0';
				module += buffer;
				WRPassiveQueue *pq = check_and_cast<WRPassiveQueue *>( getParentModule()->findObject(module.c_str(), true) );
				if( pq->length()>0 )
					pq->request(0);
			}
		 }
    }else {
		 if (jobServiced)
			 error("job arrived while already servicing one");
		 jobServiced = check_and_cast<Job *>(msg);
		 simtime_t serviceTime = par("serviceTime");
		 emit(busySignal, 1);
		 if (ev.isGUI()) getDisplayString().setTagArg("i",1,"cyan");
	 }
#else
	// pass through directly without any scheduling attempts (fastest approach)
	cModule *targetModule = getParentModule()->getSubmodule("wrRouter");
	sendDirect(msg, targetModule, "sendDirect");
#endif

} // handleMessage()

void WRServer::finish()
{
}

bool WRServer::isIdle()
{
    return jobServiced == NULL;
}

}; //namespace

