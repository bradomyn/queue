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
#include "Packet.h"

#include <vector>
#include <set>
#include <map>

namespace queueing {

class Packet;

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

        Packet *jobServiced;

        int numSent;

        // receive trigger messages
        cMessage *triggerServiceMsg;

        IPassiveQueue *getQueue(int index);
		IPassiveQueue *_q7;

		// pointer to other queues
		std::vector<IPassiveQueue*> _qs;

		vector<Packet*> _dropped;

		int _scheduling;	// see Server.ned for possible values in enum

		void priority(cMessage* msg);
		void sevenfirst(cMessage* msg);
		void feedback(cMessage* msg);

		void wfq1(cMessage* msg);
		int _weight7;


		void wfq2(cMessage* msg);
		int _rrCounter;
		int _rrN;

		void wfq3(cMessage* msg);

		void mixed1(cMessage* msg);

		map<int, IPassiveQueue*> _mapFeedback;

		const char* _schedulingAlgorithm;

    public:
        Server();
        virtual ~Server();
        vector<Packet*> getDropped(){ return _dropped; };
        const char* getSchedulingAlgorithm() { return _schedulingAlgorithm; };

    protected:
        virtual void initialize();
        virtual int numInitStages() const {return 2;}
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

    public:
        virtual bool isIdle();
};

}; //namespace

#endif


