//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_PASSIVE_QUEUE_H
#define __QUEUEING_PASSIVE_QUEUE_H

#include "QueueingDefs.h"
#include "IPassiveQueue.h"
#include "SelectionStrategies.h"

namespace queueing {

class SelectionStrategy;

/**
 * A passive queue, designed to co-operate with IServer using method calls.
 */
class QUEUEING_API PassiveQueue : public cSimpleModule, public IPassiveQueue
{
    private:
		simsignal_t droppedSignal;
		simsignal_t queueLengthSignal;
		simsignal_t queueingTimeSignal;

        bool fifo;

        // capacity of the queue
        int capacity;

        // the queue underneath
        cQueue queue;
        SelectionStrategy *selectionStrategy;	// currently not used

        // number of packets handled
        int numServed;

        // number of packets in queue
        int numQueued;
        int numQueuedIdle;

        void queueLengthChanged();

        // scheduling type
        int _scheduling;

        // check the capacity of the queue by considering the packet sizes
        void checkCapacityAndQueue(cMessage *msg);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        PassiveQueue();
        virtual ~PassiveQueue();
        // The following methods are called from IServer:
        virtual int length();
        virtual void request(int gateIndex);

        // request without a gate
        virtual void request();

        // add up packet sizes
        int determineQueueSize();

};

}; //namespace

#endif

