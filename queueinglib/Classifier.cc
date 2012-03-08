//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Classifier.h"
#include "Packet.h"

namespace queueing {

Define_Module(Classifier);

void Classifier::initialize()
{
    dispatchField = par("dispatchField");
}

void Classifier::handleMessage(cMessage *msg)
{
    Packet *packet = check_and_cast<Packet *>(msg);
    int outGateIndex = -1;
    if (strcmp(dispatchField, "type") == 0)
        outGateIndex = packet->getKind();
    else if (strcmp(dispatchField, "priority") == 0)
        outGateIndex = packet->getPriority();
    else
        error("invalid dispatchField parameter, must be \"type\" or \"priority\"");
    // TODO we could look for the value in the dynamically added parameters too

    //if (outGateIndex < 0 || outGateIndex >= gateSize("out"))
    //    send(packet, "rest");
    //else

#if 0
    send(packet, "out", outGateIndex);
    std::cout << "packet sent to " << outGateIndex << std::endl;
#else
    int prio = packet->getPriority(); //Useful::getInstance()->getPriority(packet->getName());
    send(packet, "out", prio);
    //std::cout << "packet sent to " << prio << std::endl;
#endif


}

}; //namespace

