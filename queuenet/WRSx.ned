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

import ned.DatarateChannel;



//
// A call center simulation.
//
network WRSx
{
    types:
        channel pppline extends DatarateChannel
        {
            delay = 0 s; // propagation delay
            datarate = 1000000000 bps; // bits per second 1Gbps
            disabled = false;
            ber = 0; // bit-error rate
            per = 0; // packet-error rate
        }
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
        sink1: Sink {
            @display("p=408,324;b=7,51");
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

		source.pppg++ --> pppline --> queue0.in;
		source.pppg++ --> pppline --> queue1.in;
		source.pppg++ --> pppline --> queue2.in;
		source.pppg++ --> pppline --> queue3.in;
		source.pppg++ --> pppline --> queue4.in;
		source.pppg++ --> pppline --> queue5.in;
		source.pppg++ --> pppline --> queue6.in;
		source.pppg++ --> pppline --> queue7.in;

        source.outTrigger --> server.in++;
        server.pppg++ --> pppline --> sink.in++;
        server.out --> sink1.in++;
        queue6.outTrigger --> server.in++;
        queue5.outTrigger --> server.in++;
        queue4.outTrigger --> server.in++;
        queue3.outTrigger --> server.in++;
        queue2.out++ --> server.in++;
        queue1.outTrigger --> server.in++;
        queue0.outTrigger --> server.in++;
        queue2.outTrigger --> server.in++;
        queue7.outTrigger --> server.in++;
}
