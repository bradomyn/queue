//
// Generated file, do not edit! Created by opp_msgc 4.2 from Job.msg.
//

#ifndef _JOB_M_H_
#define _JOB_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0402
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif


namespace queueing {

// cplusplus {{
#include "QueueingDefs.h"
// }}

/**
 * Class generated from <tt>Job.msg</tt> by opp_msgc.
 * <pre>
 * message Job
 * {
 *     @customize(true);
 *     int priority;                
 *     simtime_t totalQueueingTime; 
 *     simtime_t totalServiceTime;  
 *     simtime_t totalDelayTime;    
 *     int queueCount;              
 *     int delayCount;              
 *     int generation;              
 * }
 * </pre>
 *
 * Job_Base is only useful if it gets subclassed, and Job is derived from it.
 * The minimum code to be written for Job is the following:
 *
 * <pre>
 * class Job : public Job_Base
 * {
 *   private:
 *     void copy(const Job& other) { ... }

 *   public:
 *     Job(const char *name=NULL, int kind=0) : Job_Base(name,kind) {}
 *     Job(const Job& other) : Job_Base(other) {copy(other);}
 *     Job& operator=(const Job& other) {if (this==&other) return *this; Job_Base::operator=(other); copy(other); return *this;}
 *     virtual Job *dup() const {return new Job(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from Job_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(Job);
 * </pre>
 */
class Job_Base : public ::cMessage
{
  protected:
    int priority_var;
    simtime_t totalQueueingTime_var;
    simtime_t totalServiceTime_var;
    simtime_t totalDelayTime_var;
    int queueCount_var;
    int delayCount_var;
    int generation_var;

  private:
    void copy(const Job_Base& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Job_Base&);
    // make constructors protected to avoid instantiation
    Job_Base(const char *name=NULL, int kind=0);
    Job_Base(const Job_Base& other);
    // make assignment operator protected to force the user override it
    Job_Base& operator=(const Job_Base& other);

  public:
    virtual ~Job_Base();
    virtual Job_Base *dup() const {throw cRuntimeError("You forgot to manually add a dup() function to class Job");}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getPriority() const;
    virtual void setPriority(int priority);
    virtual simtime_t getTotalQueueingTime() const;
    virtual void setTotalQueueingTime(simtime_t totalQueueingTime);
    virtual simtime_t getTotalServiceTime() const;
    virtual void setTotalServiceTime(simtime_t totalServiceTime);
    virtual simtime_t getTotalDelayTime() const;
    virtual void setTotalDelayTime(simtime_t totalDelayTime);
    virtual int getQueueCount() const;
    virtual void setQueueCount(int queueCount);
    virtual int getDelayCount() const;
    virtual void setDelayCount(int delayCount);
    virtual int getGeneration() const;
    virtual void setGeneration(int generation);
};

}; // end namespace queueing

#endif // _JOB_M_H_
