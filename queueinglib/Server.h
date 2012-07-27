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
#include "Packet_m.h"

#include <vector>
#include <set>
#include <map>

using std::vector;
using std::set;
using std::map;

using std::cout;
using std::endl;

namespace queueing {

class WRPacket;

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

        WRPacket *jobServiced;

        int numSent;

        // receive trigger messages
        cMessage *triggerServiceMsg;

        cGate *outputgate;
        cDatarateChannel *channel;
        cQueue *outQueue;
        cMessage *endServiceMsg;

        IPassiveQueue *getQueue(int index);
		IPassiveQueue *_q7;

		// pointer to other queues
		std::vector<IPassiveQueue*> _qs;

		vector<WRPacket*> _dropped;

		int _scheduling;	// see Server.ned for possible values in enum

		int _nofCoS;	// default: 7..0

		void sendPacket(WRPacket* packet);
		void sendPacketOverChannel(WRPacket* packet);

		void original(cMessage* msg);

		void priority(cMessage* msg);
		void sevenfirst(cMessage* msg);
		void lqf(cMessage* msg);

		void wfq1(cMessage* msg);
		int _weight7;


		void wfq2(cMessage* msg);
		int _rrCounter;
		int _rrN;

		void wfq3(cMessage* msg);
		void wfq4(cMessage* msg);

		void mixed1(cMessage* msg);

		map<int, IPassiveQueue*> _mapQSizes;
		std::map<simtime_t, IPassiveQueue*> _map;

		const char* _schedulingAlgorithm;

		void fcfs(cMessage* msg);
		vector<int> _fcfsQueueServeList;

    public:
        Server();
        virtual ~Server();
        vector<WRPacket*> getDropped(){ return _dropped; };
        const char* getSchedulingAlgorithm() { return _schedulingAlgorithm; };

        void add2QueueServeList(int nr){
        	_fcfsQueueServeList.push_back(nr);
        }

        int getFromQueueServeList() {
        	if( _fcfsQueueServeList.size()>0 )
        		return _fcfsQueueServeList.at(0);
        	else return -1;
        }

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


