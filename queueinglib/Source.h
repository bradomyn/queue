//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_SOURCE_H
#define __QUEUEING_SOURCE_H

#include "QueueingDefs.h"
#include "Job.h"
#include "Useful.h"

namespace queueing {

class Job;

/**
 * Abstract base class for job generator modules
 */
class QUEUEING_API SourceBase : public cSimpleModule
{
    protected:
        int jobCounter;
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
class QUEUEING_API Source : public SourceBase
{
    private:
        simtime_t startTime;
        simtime_t stopTime;
        int numJobs;
        int numCreated;
        std::vector<JobDescription> _data;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

        Job * generateJob();
        Job * generateJob( int priority, int size );
};

};

#endif


