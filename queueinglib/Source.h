//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_SOURCE_H
#define __QUEUEING_SOURCE_H

#include "QueueingDefs.h"
#include "Sink.h"
#include "Packet.h"
#include "Useful.h"

namespace queueing {

class Sink;
class Packet;

/**
 * Abstract base class for job generator modules
 */
class QUEUEING_API SourceBase : public cSimpleModule
{
    protected:
        int jobCounter;
        std::string packetName;
        simsignal_t createdSignal;
        int packetCounter;

    protected:
        virtual void initialize();
        virtual Packet *createJob();
        virtual void finish();
};


/**
 * Generates jobs; see NED file for more info.
 */
class QUEUEING_API Source : public SourceBase
{
    private:
        simtime_t startTime;
        simtime_t stopTime;
        //int numJobs;
        int numPackets;
        int numCreated;
        std::vector<PacketDescription> _data;

        Sink *_sink;
        std::vector<int> _sent;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

        Packet * generatePacket();
        Packet * generatePacket( int priority, int size );
        void send2Queue(Packet* packet);

    public:
    	int getCreated() { return numCreated; };
    	std::vector<int> getSent() { return _sent; };
};


/**
 * Generates jobs; see NED file for more info.
 */
class QUEUEING_API SourceOnce : public SourceBase
{
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

}; //namespace

#endif


