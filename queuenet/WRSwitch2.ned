//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

import org.omnetpp.queueing.Classifier;
import org.omnetpp.queueing.WRClassifier;
import org.omnetpp.queueing.Delay;
import org.omnetpp.queueing.PassiveQueue;
import org.omnetpp.queueing.WRServer;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;

// SMa, 13.01.2012
import org.omnetpp.queueing.WRRouter;
import org.omnetpp.queueing.WRSource;
import org.omnetpp.queueing.WRQueue;
//
// Second try to simulate the current scheduling within the White Rabbit Switch.
// 
//
network WhiteRabbitSwitch2
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
        classifier: WRClassifier {
            @display("p=239,365;b=27,28");
        }
        wrQueue: WRQueue {
            @display("p=288,295");
        }
    connections:
        in.out --> classifier.in++;
        in1.out --> classifier.in++;
        in2.out --> classifier.in++;
        in3.out --> classifier.in++;
        in4.out --> classifier.in++;
        in5.out --> classifier.in++;
        in6.out --> classifier.in++;
        switch.out[0] --> out.in++;
        in7.out --> classifier.in++;
        classifier.out++ --> switch.in++;
        classifier.out++ --> wrQueue.in++;
        wrQueue.out --> switch.in++;
}
