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

namespace queueing {

class Packet;
class SelectionStrategyServer;

/**
 * The queue server. It cooperates with several Queues which queue up
 * the packets, and send them to Server on request.
 *
 * @see PassiveQueue
 */

class QUEUEING_API Server : public cSimpleModule, public IServer
{
    private:
		simsignal_t busySignal;

		// number of queues
        int numQueues;
        SelectionStrategyServer *selectionStrategy;	// currently not used

        // the current packet scheduled
        Packet *packetServiced;

        // internal trigger for the server
        cMessage *endServiceMsg;

        // receive trigger messages
        cMessage *triggerServiceMsg;

        int numSent;
        int triggerCounter;

        // service time of ther server
        simtime_t _serviceTime;

        // feedback: contains packets in their arrival order
        map<simtime_t, Packet*> _order;
        //http://www.cplusplus.com/reference/stl/map/: Internally, the elements in the map are sorted from lower to higher key value
        //following a specific strict weak ordering criterion set on construction.


		// feedback2, internal 'queues'
		vector<Packet *> _iq7;
		vector<Packet *> _iq6;
		vector<Packet *> _iq5;
		vector<Packet *> _iq4;
		vector<Packet *> _iq3;
		vector<Packet *> _iq2;
		vector<Packet *> _iq1;
		vector<Packet *> _iq0;

		// feedback2: check the waiting time of a packet and move it to another queue if timeDist criteria is fulfilled
		void checkWaitingTimeAndCapacityAndMoveToOtherQueue(int priority, vector<Packet*> &v1, vector<Packet*> &v2, simtime_t timeDist);

		// compute queue size from contents (packet sizes)
		int determineQueueSize(vector<Packet*> v);

		// queues maximum capacity
		int _capacity;

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
        // send the current packet in packetServiced
        void serveCurrentPacket();

        // retrieve a pointer to queue with given index
        IPassiveQueue *getQueue(int index);
        IPassiveQueue *_q7;

        // pointer to other queues
        std::vector<IPassiveQueue*> _qs;

        // Server.ned: string schedulingAlgorithm @enum("none", "priority" , "feedback", "original") = default("none");
        int _scheduling;	// see Server.ned for possible values in enum
};

}; //namespace

#endif


