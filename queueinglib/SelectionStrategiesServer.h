//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_SELECTIONSTRATEGIES_SERVER_H
#define __QUEUEING_SELECTIONSTRATEGIES_SERVER_H

#include "QueueingDefs.h"

namespace queueing {

/**
 * Selection strategies used in queue, server and router classes to decide
 * which module to choose for further interaction.
 */
class QUEUEING_API SelectionStrategyServer : public cObject
{
    protected:
        bool isInputGate;
        int gateSize;        // the size of the gate vector
        cModule *hostModule; // the module using the strategy
    public:
        // on which module's gates should be used for selection
        // if selectOnInGate is true, then we will use "in" gate otherwise "out" is used
        SelectionStrategyServer(cSimpleModule *module, bool selectOnInGate);
        virtual ~SelectionStrategyServer();

        static SelectionStrategyServer * create(const char *algName, cSimpleModule *module, bool selectOnInGate);

        // which gate index the selection strategy selected
        virtual int select() = 0;
        // returns the i-th module's gate which connects to our host module
        cGate *selectableGate(int i);
    //protected:
        // is this module selectable according to the policy? (queue is selectable if not empty, server is selectable if idle)
        virtual bool isSelectable(cModule *module);
};

/**
 * Priority based selection. The first selectable index will be returned.
 */
class QUEUEING_API PrioritySelectionStrategyServer : public SelectionStrategyServer
{
    public:
        PrioritySelectionStrategyServer(cSimpleModule *module, bool selectOnInGate);
        virtual int select();
};

/**
 * Random selection from the selectable modules, with uniform distribution.
 */
class QUEUEING_API RandomSelectionStrategyServer : public SelectionStrategyServer
{
    public:
        RandomSelectionStrategyServer(cSimpleModule *module, bool selectOnInGate);
        virtual int select();
};

/**
 * Uses Round Robin selection, but skips any module that is not available currently.
 */
class QUEUEING_API RoundRobinSelectionStrategyServer : public SelectionStrategyServer
{
    protected:
        int lastIndex; // the index of the module last time used
    public:
        RoundRobinSelectionStrategyServer(cSimpleModule *module, bool selectOnInGate);
        virtual int select();
};

/**
 * Chooses the shortest queue. If there are more than one
 * with the same length, it chooses by priority among them.
 * This strategy is for output only (i.e. for router module).
 */
class QUEUEING_API ShortestQueueSelectionStrategyServer : public SelectionStrategyServer
{
    public:
        ShortestQueueSelectionStrategyServer(cSimpleModule *module, bool selectOnInGate);
        virtual int select();
};

/**
 * Chooses the longest queue (where length>0 of course).
 * Input strategy (for servers).
 */
class QUEUEING_API LongestQueueSelectionStrategyServer : public SelectionStrategyServer
{
    public:
        LongestQueueSelectionStrategyServer(cSimpleModule *module, bool selectOnInGate);
        virtual int select();
};

/**
 * Treats high priority packets first
 */
class QUEUEING_API WRSSelectionStrategyServer1 : public SelectionStrategyServer
{
    public:
        WRSSelectionStrategyServer1(cSimpleModule *module, bool selectOnInGate);
        virtual int select();
};

}; //namespace

#endif

