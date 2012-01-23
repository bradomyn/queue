//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

// SMa, 11.01.2012

#include "WRClassifier_out.h"
#include "WRQueue.h"
#include "Job.h"

namespace queueing {

Define_Module(WRClassifier_out);

void WRClassifier_out::initialize()
{
    dispatchField = par("dispatchField");
}

void WRClassifier_out::handleMessage(cMessage *msg)
{
    Job *job = check_and_cast<Job *>(msg);
    int priority = job->getPriority();
    int outGateIndex = -1;
    if (strcmp(dispatchField, "type") == 0)
        outGateIndex = job->getKind();
    else if (strcmp(dispatchField, "priority") == 0) {
        outGateIndex = job->getPriority();
        std::cout << "priority " << job->getPriority() << std::endl;
    }
    else
        error("invalid dispatchField parameter, must be \"type\" or \"priority\"");
    // TODO we could look for the value in the dynamically added parameters too

    if( priority==7 ) {
		std::string out;
		out = "out";
		char buffer[3];

		sprintf(buffer,"%d",priority);
		buffer[2]='\0';
		//std::cout << "buf " << buffer << std::endl;
		out += buffer;
		std::cout << "Classifier out: send package with priority " << priority << " to " << out << std::endl;
		cModule *targetModule = getParentModule()->getSubmodule(out.c_str());
		sendDirect(msg, targetModule, "sendDirect");
    //} else {

		// TODO check all queues, retrieve elements from queues
		std::string queue;
		queue = "wrQueue";

		for( int i=0; i<8; i++ ) {
			WRQueue *q = retrieveQueue(i);

			if( !q->getQueue().isEmpty() )
				std::cout << "#objects in " << q->getName() << ": " << q->getQueue().length() << std::endl;
#if 1
			// empty queues
			while( q->getQueue().length()>0 ) {
				std::cout << "empty all queues " << std::endl;
				Job *job = q->getFromQueue();
				q->startService(job);
			}
#else
			if( i==7 )
			{
				if( q->getQueue().isEmpty() )
				{
					std::cout << "empty other queues " << std::endl;
					// empty other queues
					for( int j=0; j<7; j++ ) {
						WRQueue *qq = retrieveQueue(j);
						while( qq->getQueue().length()>0 ) {
							Job *job = qq->getFromQueue();
							qq->startService(job);
						}
					}
				}
			}
#endif
		}
    } else {
    	std::cout << "send to sink: " << job->getName() << std::endl;
    	send(job, "out", outGateIndex);
    }
}

WRQueue *WRClassifier_out::retrieveQueue(int index) {
	std::string queue;
	queue = "wrQueue";
	char buffer[3];
	sprintf(buffer,"%d",index);
	buffer[2]='\0';
	queue += buffer;
	WRQueue *q = check_and_cast<WRQueue *>( getParentModule()->findObject(queue.c_str(), true) );
	if( q!=NULL )
		return q;
	else return NULL;
}

}; //namespace

