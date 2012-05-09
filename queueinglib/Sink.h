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
#include "Useful.h"

#include <vector>

using namespace std;

namespace queueing {

/**
 * Consumes packets; see NED file for more info.
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
    bool keepPackets;

    int numReceived;

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

    // group for packet sizes
    vector<int> v00;
    vector<int> v01;
    vector<int> v02;
    vector<int> v03;
    vector<int> v04;
    vector<int> v05;
    vector<int> v06;
    vector<int> v07;

    simsignal_t lifeTimeSignal0;
    simsignal_t lifeTimeSignal1;
    simsignal_t lifeTimeSignal2;
    simsignal_t lifeTimeSignal3;
    simsignal_t lifeTimeSignal4;
    simsignal_t lifeTimeSignal5;
    simsignal_t lifeTimeSignal6;
    simsignal_t lifeTimeSignal7;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

  private:
    void determineQueueSizes();


};

}; //namespace

#endif

