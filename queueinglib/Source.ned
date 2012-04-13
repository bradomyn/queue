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
// A module that generates packets. One can specify the number of packets to be generated,
// the starting and ending time, and interval between generating packets.
// Packet generation stops when the number of packets or the end time has been reached,
// whichever occurs first. The name, type and priority of packets can be set as well.
//
simple Source
{
    parameters:
        @group(Queueing);
        @signal[created](type="int");
        @statistic[created](title="the number of packets created";record=count;interpolationmode=none);
        @display("i=block/source");
        int numPackets = default(-1);               // number of packets to be generated (-1 means no limit)
        volatile double interArrivalTime @unit(s); // time between generated packets
        string packetName = default("packet");         // the base name of the generated packet (will be the module name if left empty)
        volatile int packetType = default(0);       // the type attribute of the created packet (used by classifers and other modules)
        volatile int packetPriority = default(0);   // priority of the packet
        double startTime @unit(s) = default(interArrivalTime); // when the module sends out the first packet
        double stopTime @unit(s) = default(-1s); // when the module stops the packet generation (-1 means no limit)
        
        string inputDataFile = default("data.txt");
        
    gates:
        output out;
        output outTrigger;
}

