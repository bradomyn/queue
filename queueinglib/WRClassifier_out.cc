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

	std::string out;
	out = "out";
	char buffer[3];

#ifdef __linux__
		//itoa (i,buffer,10);
        sprintf(buffer,"%d",priority);
#else
        itoa (priority,buffer,10);
#endif
	buffer[2]='\0';
	std::cout << "buf " << buffer << std::endl;
	out += buffer;
	std::cout << "Classifier out: send package with priority " << priority << " to " << out << std::endl;
	cModule *targetModule = getParentModule()->getSubmodule(out.c_str());
	sendDirect(msg, targetModule, "sendDirect");

    // SMa, 13.01.2012
    //if( priority<7 ) {
    /*	if (outGateIndex < 0 || outGateIndex >= gateSize("out")) {
			//send(job, "rest");
			//error("outGateIndex < 0 || outGateIndex >= gateSize(out): %d (%d)", outGateIndex, gateSize("out"));
			std::cout << " outGateIndex: " << outGateIndex << " prio " << priority << std::endl;
			//send(job, "out", 0);	// danger of overriding something?


		}*/
    //} else {
    	// packets with highest priority send immediately
		//cModule *targetModule = getParentModule()->getSubmodule("switch");
		//sendDirect(msg, targetModule, "sendDirect");
    //}

    /*
    if (outGateIndex < 0 || outGateIndex >= gateSize("out")) {
        //send(job, "rest");
        //error("outGateIndex < 0 || outGateIndex >= gateSize(out): %d (%d)", outGateIndex, gateSize("out"));
        std::cout << "outGateIndex: " << outGateIndex << std::endl;
    	//send(job, "out", 0);	// danger of overriding something?

        cModule *targetModule = getParentModule()->getSubmodule("wrQueue");
    	sendDirect(msg, targetModule, "sendDirect");
    }
    else if (outGateIndex>=7) {
    	// packets with highest priority send immediately
    	//send(job, "out", 0);	// danger of overriding something?
    	cModule *targetModule = getParentModule()->getSubmodule("switch");
		sendDirect(msg, targetModule, "sendDirect");
    }
    else {
    	// insert into queue that should be emptied if there is no high priority packet
    	// TODO
    	//send(job, "out", outGateIndex);
    	cModule *targetModule = getParentModule()->getSubmodule("wrQueue");
		sendDirect(msg, targetModule, "sendDirect");
    }*/
}

}; //namespace

