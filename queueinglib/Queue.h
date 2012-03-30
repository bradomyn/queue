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
#include "Useful.h"

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

        Packet *packetServiced;
        cMessage *endServiceMsg;
        cQueue queue;

        // capacity of the queue
        int capacity;

        // capacity of the queue
        int _capacity;

        bool fifo;

        Packet *getFromQueue();

        // number of packets handled
        int numServed;

        // number of packets in queue
        int numQueued;

        // scheduling type
        int _scheduling;

        // check the capacity of the queue by considering the packet sizes
        void checkCapacityAndQueue(cMessage *msg);


    public:
        Queue();
        virtual ~Queue();
        int length();

        // request without a gate
        virtual void request();

        // add up packet sizes
        int determineQueueSize();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

        // hook functions to (re)define behaviour
        virtual void arrival(Packet *packet);
        virtual simtime_t startService(Packet *packet);
        virtual void endService(Packet *packet);
};

}; //namespace

#endif
