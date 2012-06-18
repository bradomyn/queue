//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

import org.omnetpp.queueing.PassiveQueue;
import org.omnetpp.queueing.Server;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;


//
// A call center simulation.
//
network WRS6
{
    @display("bgb=454,472");
    submodules:
        source: Source {
            @display("p=38,236");
        }
        queue0: PassiveQueue {
            @display("p=225.0,27.0");
        }
        queue1: PassiveQueue {
            @display("p=225,84");
        }
        queue2: PassiveQueue {
            @display("p=225,142");
        }
        queue3: PassiveQueue {
            @display("p=225,300");
        }
        server: Server {
            @display("p=338,236");
        }
        sink: Sink {
            @display("p=418,236");
        }
        queue4: PassiveQueue {
            @display("p=225,365");
        }
        queue5: PassiveQueue {
            @display("p=225,430");
        }
    connections:
        queue0.out++ --> server.in++;
        queue1.out++ --> server.in++;
        queue2.out++ --> server.in++;
        queue3.out++ --> server.in++;
        queue4.out++ --> server.in++;
        queue5.out++ --> server.in++;
        server.out --> sink.in++;
        source.outTrigger --> server.in++;
        source.out++ --> queue0.in;
        source.out++ --> queue1.in;
        source.out++ --> queue2.in;
        source.out++ --> queue3.in;
        source.out++ --> queue4.in;
        source.out++ --> queue5.in;
}
