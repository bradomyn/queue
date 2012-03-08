//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_TRIGGER_H
#define __QUEUEING_TRIGGER_H

#include "QueueingDefs.h"
#include "Packet.h"
#include "Useful.h"

namespace queueing {

class Packet;

/**
 * Abstract base class for packet generator modules
 */
class QUEUEING_API TriggerBase : public cSimpleModule
{
    protected:
        int triggerCounter;
        simsignal_t createdSignal;
    protected:
        virtual void initialize();
        virtual Packet *createPacket();
        virtual void finish();
};


/**
 * Generates packets; see NED file for more info.
 */
class QUEUEING_API Trigger : public TriggerBase
{
    private:
        simtime_t startTime;
        simtime_t stopTime;
        int numPackets;
        int numCreated;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

        Packet * generateTrigger();
};

};

#endif


