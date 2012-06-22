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
// Queue server. It serves multiple input queues (PassiveQueue), using a preset
// algorithm. Inputs must be connected to Passive Queues (PassiveQueue)
//
simple Server
{
    parameters:
        @group(Queueing);
        @display("i=block/server");
        @signal[busy](type="bool");
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);

        // how the next job will be choosen from the attached queues
        string schedulingAlgorithm @enum("original", "priority" , "lqf", "7first", "wfq1", "wfq2", "wfq3", "wfq4", "mixed1", "mixed2", "wred1") = default("original");
       
        volatile double serviceTime @unit(s); // service time of a job
        
        int weight7 = default(4);	// weight used in wfq1 
        int rrN = default(5);		// weight used in wfq2
        
        int nofCoS = default(8);	// default: 7..0
    gates:
        input in[];
        output out;
}


