//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

// SMa, 11.01.2012

#ifndef __QUEUEING_WRCLASSIFIER_OUT_H
#define __QUEUEING_WRCLASSIFIER_OUT_H

#include "QueueingDefs.h"

#include "WRQueue.h"

namespace queueing {

/**
 * See the NED declaration for more info.
 */
class QUEUEING_API WRClassifier_out : public cSimpleModule
{
    private:
        const char *dispatchField;   // the message's field or parameter we are dispatching on

        WRQueue *retrieveQueue(int index);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

}; //namespace

#endif
