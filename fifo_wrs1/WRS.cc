//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


#include "WRS.h"

namespace wrs {

Define_Module(WRS);

simtime_t WRS::startService(cMessage *msg)
{
    EV << "Starting service of " << msg->getName() << endl;
    //std::cout << "Starting service of " << msg->getName() << std::endl;
    return par("serviceTime");
}

void WRS::endService(cMessage *msg)
{
    //EV << "Completed service of " << msg->getName() << endl;
    //std::cout << "Completed service of " << msg->getName() << std::endl;

    // extract priority from jobname
    std::string jobname = std::string(msg->getName());
	int prio=Useful::getInstance()->getPriority(jobname);
	// deliver to matching sink
    send( msg, "out", prio );
    // treat prio 7 first in abstractwrs
}

}; //namespace

