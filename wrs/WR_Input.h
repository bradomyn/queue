//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_WR_INPUT_H
#define __QUEUEING_WR_INPUT_H


#include "QueueingDefs.h"



namespace queueing {

class Job;

/**
 * Abstract base class for job generator modules
 */
class QUEUEING_API WR_InputBase : public cSimpleModule
{
    protected:
        int jobCounter;
        int counter;	// SMa, 13.01.2012
        std::string jobName;
        simsignal_t createdSignal;
    protected:
        virtual void initialize();
        virtual Job *createJob();
        virtual void finish();
};


/**
 * Generates jobs; see NED file for more info.
 */
class QUEUEING_API WR_Input : public WR_InputBase
{
    private:
        //simtime_t startTime;
        //simtime_t stopTime;
        int numJobs;

        // SMa, 18.01.2012
        //cSocketRTScheduler *rtScheduler;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};


/**
 * Generates jobs; see NED file for more info.
 */
class QUEUEING_API WR_InputOnce : public WR_InputBase
{
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

}; //namespace

#endif


