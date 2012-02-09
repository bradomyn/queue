//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

// SMa, 11.01.2012

#include "WRClassifier_in.h"
#include "Job.h"

namespace queueing {

Define_Module(WRClassifier_in);

void WRClassifier_in::initialize()
{
	numSent = 0;
	WATCH(numSent);

    dispatchField = par("dispatchField");
}

void WRClassifier_in::handleMessage(cMessage *msg)
{
    Job *job = check_and_cast<Job *>(msg);
    int priority = job->getPriority();
    /*int outGateIndex = -1;
    if (strcmp(dispatchField, "type") == 0)
        outGateIndex = job->getKind();
    else if (strcmp(dispatchField, "priority") == 0) {
        outGateIndex = job->getPriority();
        std::cout << "priority " << job->getPriority() << std::endl;
    }
    else
        error("invalid dispatchField parameter, must be \"type\" or \"priority\"");*/

//    if (outGateIndex < 0 || outGateIndex >= gateSize("out"))
//        send(job, "rest");
//    else
//        send(job, "out", outGateIndex);

    std::string queue;
	queue = "wrQueue";
	char buffer[3];

    sprintf(buffer,"%d",priority);
    buffer[2]='\0';
	queue += buffer;
	//std::cout << __FILE__ << " send " << job->getName() << " to " << queue << std::endl;
	cModule *targetModule = getParentModule()->getSubmodule(queue.c_str());
	sendDirect(msg, targetModule, "sendDirect");
	numSent++;

}

}; //namespace

