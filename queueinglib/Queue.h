//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_QUEUE_H
#define __QUEUEING_QUEUE_H

#include "QueueingDefs.h"

namespace queueing {

class Packet;

/**
 * Abstract base class for single-server queues.
 */
class QUEUEING_API Queue : public cSimpleModule
{
    private:
		simsignal_t droppedSignal;
		simsignal_t queueLengthSignal;
		simsignal_t queueingTimeSignal;
		simsignal_t busySignal;

        Packet *jobServiced;
        cMessage *endServiceMsg;
        cQueue queue;
        int capacity;
        bool fifo;

        Packet *getFromQueue();

    public:
        Queue();
        virtual ~Queue();
        int length();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

        // hook functions to (re)define behaviour
        virtual void arrival(Packet *job);
        virtual simtime_t startService(Packet *job);
        virtual void endService(Packet *job);
};

}; //namespace

#endif
