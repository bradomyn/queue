//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_WRQUEUE_H
#define __QUEUEING_WRQUEUE_H

#include "QueueingDefs.h"

namespace queueing {

class Job;

/**
 * Abstract base class for single-server queues.
 */
class QUEUEING_API WRQueue : public cSimpleModule
{
    private:
		simsignal_t droppedSignal;
		simsignal_t queueLengthSignal;
		simsignal_t queueingTimeSignal;
		simsignal_t busySignal;

        Job *jobServiced;
        cMessage *endServiceMsg;
        cQueue queue;
        int capacity;
        bool fifo;

        Job *getFromQueue();

    public:
        WRQueue();
        virtual ~WRQueue();
        int length();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

        // hook functions to (re)define behaviour
        virtual void arrival(Job *job);
        virtual simtime_t startService(Job *job);
        virtual void endService(Job *job);
};

}; //namespace

#endif
