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
// A module that generates trigger events. One can specify the number of trigger events to be generated,
// the starting and ending time, and interval between generating trigger events.
// Trigger event generation stops when the number of trigger events or the end time has been reached,
// whichever occurs first.
//
simple Trigger
{
    parameters:
        @group(Queueing);
        @signal[created](type="int");
        @statistic[created](title="the number of packets created";record=count;interpolationmode=none);
        @display("i=block/trigger");
        int numPackets = default(-1);               // number of packets to be generated (-1 means no limit)
        volatile double triggerIntervall @unit(s); // time between generated packets
        //string packetName = default("packet");         // the base name of the generated packet (will be the module name if left empty)
        //volatile int packetType = default(0);       // the type attribute of the created packet (used by classifers and other modules)
        //volatile int packetPriority = default(0);   // priority of the packet
        double startTime @unit(s) = default(triggerIntervall); // when the module sends out the first packet
        double stopTime @unit(s) = default(-1s); // when the module stops the packet generation (-1 means no limit)
        
    gates:
        output out;
}

