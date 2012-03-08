//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_SERVER_H
#define __QUEUEING_SERVER_H

#include "IServer.h"
#include "Useful.h"

#include "PassiveQueue.h"

#include <vector>

namespace queueing {

class Job;
class SelectionStrategyServer;

/**
 * The queue server. It cooperates with several Queues that which queue up
 * the jobs, and send them to Server on request.
 *
 * @see PassiveQueue
 */
class QUEUEING_API Server : public cSimpleModule, public IServer
{
    private:
		simsignal_t busySignal;

        int numQueues;
        SelectionStrategyServer *selectionStrategy;	// currently not used

        Job *jobServiced;
        cMessage *endServiceMsg;

        // receive trigger messages
        cMessage *triggerServiceMsg;

        int numSent;
        int triggerCounter;

    public:
        Server();
        virtual ~Server();

    protected:
        virtual void initialize();
        virtual int numInitStages() const {return 2;}
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

    public:
        virtual bool isIdle();

    private:
        void serveCurrentJob();

        // retrieve a pointer to queue with given index
        IPassiveQueue *getQueue(int index);
        IPassiveQueue *_q7;

        std::vector<IPassiveQueue*> _qs;

        // Server.ned: string schedulingAlgorithm @enum("none", "priority" , "feedback", "original") = default("none");
        int _scheduling;	// see Server.ned for possible values in enum
};

}; //namespace

#endif


