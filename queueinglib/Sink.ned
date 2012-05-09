//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package org.omnetpp.queueing;

//
// Destroys (or optionally keep) the packets and collects statistics
//
simple Sink
{
    parameters:
        @group(Queueing);
        @display("i=block/sink");
        @signal[lifeTime](type="simtime_t");
        @signal[totalQueueingTime](type="simtime_t");
        @signal[totalDelayTime](type="simtime_t");
        @signal[totalServiceTime](type="simtime_t");
        @signal[queuesVisited](type="int");
        @signal[delaysVisited](type="int");
        @signal[generation](type="int");
        
        @signal[lifeTime0](type="simtime_t");
        @signal[lifeTime1](type="simtime_t");
        @signal[lifeTime2](type="simtime_t");
        @signal[lifeTime3](type="simtime_t");
        @signal[lifeTime4](type="simtime_t");
        @signal[lifeTime5](type="simtime_t");
        @signal[lifeTime6](type="simtime_t");
        @signal[lifeTime7](type="simtime_t");
        @statistic[lifeTime0](title="lifetime of arrived packets prio 0";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[lifeTime1](title="lifetime of arrived packets prio 1";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[lifeTime2](title="lifetime of arrived packets prio 2";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[lifeTime3](title="lifetime of arrived packets prio 3";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[lifeTime4](title="lifetime of arrived packets prio 4";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[lifeTime5](title="lifetime of arrived packets prio 5";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[lifeTime6](title="lifetime of arrived packets prio 6";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[lifeTime7](title="lifetime of arrived packets prio 7";unit=s;record=vector,mean,max;interpolationmode=none);
        
        @statistic[lifeTime](title="lifetime of arrived packets";unit=s;record=vector,mean,max;interpolationmode=none);
        @statistic[totalQueueingTime](title="the total time spent in queues by arrived packets";unit=s;record=vector?,mean,max;interpolationmode=none);
        @statistic[totalDelayTime](title="the total time spent in delay nodes by arrived packets";unit=s;record=vector?,mean,max;interpolationmode=none);
        @statistic[totalServiceTime](title="the total time spent  by arrived packets";unit=s;record=vector?,mean,max;interpolationmode=none);
        @statistic[queuesVisited](title="the total number of queues visited by arrived packets";record=vector?,mean,max;interpolationmode=none);
        @statistic[delaysVisited](title="the total number of delays visited by arrived packets";record=vector?,mean,max;interpolationmode=none);
        @statistic[generation](title="the generation of the arrived packets";record=vector?,mean,max;interpolationmode=none);
        bool keepPackets = default(false); // whether to keep the received packets till the end of simulation
    gates:
        input in[];
}

