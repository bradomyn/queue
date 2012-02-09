//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_WRSOURCE7_H
#define __QUEUEING_WRSOURCE7_H



#include "QueueingDefs.h"

#ifdef __linux__
#include <sys/time.h>
#include <sys/types.h>
#endif

#include "Timer.h"
#include "WRPacket.h"

namespace queueing {

class Job;

/**
 * Abstract base class for job generator modules
 */
class QUEUEING_API WRSourceBase7 : public cSimpleModule
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
class QUEUEING_API WRSource7 : public WRSourceBase7
{
    private:
        simtime_t startTime;
        //simtime_t stopTime;
        int numJobs;

        long numSent;
        // SMa, 18.01.2012
        //cSocketRTScheduler *rtScheduler;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        Job * generateJob();
        std::vector<Job*> generateJobs(int number);

        double triggerTime;
};


/**
 * Generates jobs; see NED file for more info.
 */
class QUEUEING_API WRSourceOnce7 : public WRSourceBase7
{
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

}; //namespace

#endif


