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
network WRSx
{
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
            @display("p=225,197");
        }
        server: Server {
            @display("p=338,236");
        }
        sink: Sink {
            @display("p=418,236");
        }
        queue4: PassiveQueue {
            @display("p=225,255");
        }
        queue5: PassiveQueue {
            @display("p=225,311");
        }
        queue6: PassiveQueue {
            @display("p=225,369");
        }
        queue7: PassiveQueue {
            @display("p=225,431");
        }
    connections:
        queue0.out++ --> server.in++;
        queue1.out++ --> server.in++;
        queue2.out++ --> server.in++;
        queue3.out++ --> server.in++;
        queue4.out++ --> server.in++;
        queue5.out++ --> server.in++;
        queue6.out++ --> server.in++;
        queue7.out++ --> server.in++;
        server.out --> sink.in++;
        source.outTrigger --> server.in++;
        source.out++ --> queue0.in;
        source.out++ --> queue1.in;
        source.out++ --> queue2.in;
        source.out++ --> queue3.in;
        source.out++ --> queue4.in;
        source.out++ --> queue5.in;
        source.out++ --> queue6.in;
        source.out++ --> queue7.in;
}
