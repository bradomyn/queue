//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_WRROUTER_H
#define __QUEUEING_WRROUTER_H

#include "QueueingDefs.h"

namespace queueing {

// routing algorithms
enum {
     WR_ALG_RANDOM,
     WR_ALG_ROUND_ROBIN,
     WR_ALG_MIN_QUEUE_LENGTH,
     WR_ALG_MIN_DELAY,
     WR_ALG_MIN_SERVICE_TIME,
     ALG_WR_SWITCH_MACIEJ
};

/**
 * Sends the messages to different outputs depending on a set algorithm.
 */
class QUEUEING_API WRRouter : public cSimpleModule
{
    private:
        int routingAlgorithm;  // the algorithm we are using for routing
        int rrCounter;         // msgCounter for round robin routing

    // SMa, 13.01.2012
      long numSent;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

        // SMa, 13.01.2012
        virtual void updateDisplay();

        virtual void finish();
};

}; //namespace

#endif
