//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_SINK_H
#define __QUEUEING_SINK_H

#include "QueueingDefs.h"
#include "IPassiveQueue.h"
#include "PassiveQueue.h"
#include "Packet.h"
#include "Server.h"
#include "Source.h"

#include "Useful.h"
#include <vector>

using namespace std;

namespace queueing {

/**
 * Consumes jobs; see NED file for more info.
 */
class QUEUEING_API Sink : public cSimpleModule
{
  private:
	simsignal_t lifeTimeSignal;
	simsignal_t totalQueueingTimeSignal;
	simsignal_t queuesVisitedSignal;
	simsignal_t totalServiceTimeSignal;
	simsignal_t totalDelayTimeSignal;
	simsignal_t delaysVisitedSignal;
	simsignal_t generationSignal;
    bool keepJobs;

    int _nofCoS;

    int numReceived;

    void determineQueueSizes();

    // group for transfer times
	vector<simtime_t> v0;
	vector<simtime_t> v1;
	vector<simtime_t> v2;
	vector<simtime_t> v3;
	vector<simtime_t> v4;
	vector<simtime_t> v5;
	vector<simtime_t> v6;
	vector<simtime_t> v7;
	double avg_lifetime(vector<simtime_t> v);

	vector<simtime_t> vq0;
	vector<simtime_t> vq1;
	vector<simtime_t> vq2;
	vector<simtime_t> vq3;
	vector<simtime_t> vq4;
	vector<simtime_t> vq5;
	vector<simtime_t> vq6;
	vector<simtime_t> vq7;


	// group for transfer times
	vector<double> v00q;
	vector<double> v01q;
	vector<double> v02q;
	vector<double> v03q;
	vector<double> v04q;
	vector<double> v05q;
	vector<double> v06q;
	vector<double> v07q;
	double avg_lifetime(vector<double> v);

	// pointer to queues
	std::vector<PassiveQueue*> _qs;
	PassiveQueue *getQueue(int index);

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

  public:
    int getNumReceived() { return numReceived; };

};

}; //namespace

#endif

