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
    if (!selectionStrategy)
        error("invalid selection strategy");
}

void WRServer::handleMessage(cMessage *msg)
{
    if (msg==endServiceMsg)
    {
        ASSERT(jobServiced!=NULL);
        simtime_t d = simTime() - endServiceMsg->getSendingTime();
        jobServiced->setTotalServiceTime(jobServiced->getTotalServiceTime() + d);
        send(jobServiced, "out");
        jobServiced = NULL;
        emit(busySignal, 0);

        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

        // examine all input queues, and request a new job from a non empty queue
        int k = selectionStrategy->select();
        EV << "selected new job " << k << endl;

        // TODO
        // SMa, 11.01.2012 don't request packets from queues
        if (k >= 0)
        {
            EV << "selected new job " << k << endl;
            //EV << "requesting job from queue " << k << endl;
        //    cGate *gate = selectionStrategy->selectableGate(k);
        //    check_and_cast<IPassiveQueue *>(gate->getOwnerModule())->request(gate->getIndex());
        }
    }
    else
    {
        if (jobServiced)
            error("job arrived while already servicing one");

        jobServiced = check_and_cast<Job *>(msg);
        simtime_t serviceTime = par("serviceTime");
        scheduleAt(simTime()+serviceTime, endServiceMsg);
        emit(busySignal, 1);

        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"cyan");
    }
}

void WRServer::finish()
{
}

bool WRServer::isIdle()
{
    return jobServiced == NULL;
}

}; //namespace
