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

        map<simtime_t, Packet*> _order7;

		// feedback2, internal 'queues'
		vector<Packet *> _iq7;
		vector<Packet *> _iq6;
		vector<Packet *> _iq5;
		vector<Packet *> _iq4;
		vector<Packet *> _iq3;
		vector<Packet *> _iq2;
		vector<Packet *> _iq1;
		vector<Packet *> _iq0;

		// for algorithm 7first
		vector<Packet *> _iqX;

		// feedback2: check the waiting time of a packet and move it to another queue if timeDist criteria is fulfilled
		void checkWaitingTimeAndCapacityAndMoveToOtherQueue(int priority, vector<Packet*> &v1, vector<Packet*> &v2, simtime_t timeDist);

		void checkQueueSizeAndSend(vector<Packet*> &v);

		// compute queue size from contents (packet sizes)
		int determineQueueSize(vector<Packet*> v);

		// queues maximum capacity
		int _capacity;

		void none(cMessage *msg);
		void priority(cMessage *msg);
		void feedback1(cMessage *msg);
		void feedback2(cMessage *msg);
		void feedback3(cMessage *msg);
		void seven_first(cMessage *msg);
		void original(cMessage *msg);
		void wfq1(cMessage *msg);
		void wfq4(cMessage *msg);
		void mixed1(cMessage *msg);
		void mixed2(cMessage *msg);
		void wred1(cMessage *msg);
		void pushPacket2Queue(Packet *p);
		void pushPacket2QueueCheckNofPackets(Packet *p);
		void pushPacket2QueueCheckSize(Packet *p);
		void avgQueueSizeWRED(Packet* p);
		void pushPacket2QueueWRED(Packet *p);
		vector<Packet*> _dropped;

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
        vector<Packet* > getDropped() { return _dropped; };

    private:
        // send the current packet in packetServiced
        void serveCurrentPacket();

        // serve packet from _iqX
        void serveCurrentPacket7First();

        // Weighted Fair Queuing, 4 HP for 1 LP
        void sendWFQ1();

        // Weighted Fair Queuing, 4, 3, 2, 1 according to priority
        void sendWFQ2();

        // Weighted Fair Queuing using Round Robin
        void sendWFQ3();
        int _rrCounter;
        int _rrN;

        // Weighted Fair Queuing considering the size of a queue
        void sendWFQ4();

        void sendMixed1();
        void sendWFQ4_6to4();
        void sendMixed2();
        void sendWFQ4_3to0();

        int _nofPointersInQueue;
        int _memorySize;
        int _weightWFQ;

        void sendWRED1();
        double _weight;
        int _currentQSize;
        int _oldAvgQSize;
        int _minQSize;
        int _maxQSize;

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


