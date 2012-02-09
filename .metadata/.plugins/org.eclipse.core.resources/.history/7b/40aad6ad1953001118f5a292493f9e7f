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
network WhiteRabbitSwitch3
{
    parameters:
        @display("i=block/users;bgb=503,204");
    submodules:
        in: WRSource {
            @display("p=37,119;i=block/arrival");
        }
        switch: WRRouter {
            parameters:
                routingAlgorithm = "currentWRSwitch";
                @display("p=249,119");
            gates:
                out[sizeof(in)];
        }
        out: Sink {
            @display("p=345,119;i=block/departure");
        }
        classifier: WRClassifier {
            @display("p=138,119;b=27,28");
        }
        wrQueue: WRQueue {
            @display("p=249,43");
        }
    connections:
        in.out --> classifier.in++;
        switch.out[0] --> out.in++;
        classifier.out++ --> switch.in++;
        classifier.out++ --> wrQueue.in++;
        wrQueue.out --> switch.in++;
}
