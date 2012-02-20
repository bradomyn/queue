//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <omnetpp.h>
#include "Useful.h"
#include "../wrs/Job.h"
#include "../wrs/Timer.h"

#include <vector>

using namespace queueing;

using std::vector;
using std::string;

namespace wrs {

/**
 * Packet sink; see NED file for more info.
 */
class WRSink : public cSimpleModule {
  private:
    simsignal_t lifetimeSignal;
    vector<double> mm;	// time
    vector<simtime_t> lt;	// lifetime

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

}
