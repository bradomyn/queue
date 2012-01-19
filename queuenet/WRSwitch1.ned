//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

import org.omnetpp.queueing.WRClassifier;
import org.omnetpp.queueing.Delay;
import org.omnetpp.queueing.PassiveQueue;
import org.omnetpp.queueing.WRServer;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;

// SMa, 12.01.2012
import org.omnetpp.queueing.WRRouter;
import org.omnetpp.queueing.WRSource;
import org.omnetpp.queueing.WRQueue;
//
// Second try to simulate the current scheduling within the White Rabbit Switch.
// 
//
network WhiteRabbitSwitch1
{
    parameters:
        @display("i=block/users;bgb=679,738");
    submodules:
        in: WRSource {
            @display("p=37,39;i=block/arrival");
        }
        in1: WRSource {
            @display("p=37,132;i=block/arrival");
        }
        in2: WRSource {
            @display("p=37,225;i=block/arrival");
        }
        in3: WRSource {
            @display("p=37,318;i=block/arrival");
        }
        in4: WRSource {
            @display("p=37,411;i=block/arrival");
        }
        in5: WRSource {
            @display("p=37,504;i=block/arrival");
        }
        in6: WRSource {
            @display("p=37,597;i=block/arrival");
        }
        in7: WRSource {
            @display("p=37,690;i=block/arrival");
        }
        switch: WRRouter {
            parameters:
                routingAlgorithm = "currentWRSwitch";
                @display("p=327,365");
            gates:
                out[sizeof(in)];
        }
        out: Sink {
            @display("p=449,365;i=block/departure");
        }
        queue: WRQueue {
            parameters:
                @display("p=132,39,row,80");
        }

        queue1: WRQueue {
            parameters:
                @display("p=132,132,row,80");
        }

        queue2: WRQueue {
            parameters:
                @display("p=132,225,row,80");
        }

        queue3: WRQueue {
            parameters:
                @display("p=132,318,row,80");
        }

        queue4: WRQueue {
            parameters:
                @display("p=132,411,row,80");
        }

        queue5: WRQueue {
            parameters:
                @display("p=132,504,row,80");
        }

        queue6: WRQueue {
            parameters:
                @display("p=132,597,row,80");
        }

        queue7: WRQueue {
            parameters:
                @display("p=132,690,row,80");
        }

    connections:
        in.out --> queue.in++;
        in1.out --> queue1.in++;
        in2.out --> queue2.in++;
        in3.out --> queue3.in++;
        in4.out --> queue4.in++;
        in5.out --> queue5.in++;
        in6.out --> queue6.in++;
        in7.out --> queue7.in++;

        // connect queues to classifier (filters for high priority messages)
        queue.out --> switch.in++;
        queue1.out --> switch.in++;

        queue2.out --> switch.in++;
        queue3.out --> switch.in++;
        queue4.out --> switch.in++;
        queue5.out --> switch.in++;
        queue6.out --> switch.in++;
        queue7.out --> switch.in++;
        
        switch.out[0] --> out.in++;
}
