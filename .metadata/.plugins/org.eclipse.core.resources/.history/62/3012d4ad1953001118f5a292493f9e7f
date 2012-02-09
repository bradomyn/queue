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
//
// First try to simulate the current scheduling within the White Rabbit Switch.
// 
//
network WhiteRabbitSwitch
{
    parameters:
        @display("i=block/users");
    submodules:
        in: Source {
            @display("p=36,365;i=block/arrival");
        }
        classifier: WRClassifier {
            @display("p=120,365");
        }
        //toOtherTerminal: Sink {
        //    @display("p=124.0,225.0");
        //}
        input_block0: Delay {
            @display("p=222.0,39.0");
        }
        input_block1: Delay {
            @display("p=222.0,132.0");
        }
        input_block2: Delay {
            @display("p=222.0,225.0");
        }
        input_block3: Delay {
            @display("p=222.0,318.0");
        }
        input_block4: Delay {
            @display("p=222.0,411.0");
        }
        input_block5: Delay {
            @display("p=222.0,504.0");
        }
        input_block6: Delay {
            @display("p=222.0,597.0");
        }
        input_block7: Delay {
            @display("p=222.0,690.0");
        }
        output_block0: PassiveQueue {
            @display("p=329.0,39.0");
        }
        output_block1: PassiveQueue {
            @display("p=329.0,132.0");
        }
        output_block2: PassiveQueue {
            @display("p=329.0,225.0");
        }
        output_block3: PassiveQueue {
            @display("p=329.0,318.0");
        }
        output_block4: PassiveQueue {
            @display("p=329.0,411.0");
        }
        output_block5: PassiveQueue {
            @display("p=329.0,504.0");
        }
        output_block6: PassiveQueue {
            @display("p=329.0,597.0");
        }
        output_block7: PassiveQueue {
            @display("p=329.0,690.0");
        }
        checkin: WRServer {
            @display("p=448,365");
        }
        out: Sink {
            @display("p=538,365;i=block/departure");
        }
        switch: WRRouter {
            parameters:
                routingAlgorithm = "random";
                @display("p=329.0,783.0");
            gates:
                out[sizeof(in)];
        }
    connections:
        in.out --> classifier.in++;
        classifier.out++ --> input_block0.in++;
        classifier.out++ --> input_block1.in++;
        classifier.out++ --> input_block2.in++;
        classifier.out++ --> input_block3.in++;
        classifier.out++ --> input_block4.in++;
        classifier.out++ --> input_block5.in++;
        classifier.out++ --> input_block6.in++;
        classifier.out++ --> input_block7.in++;
        output_block0.out++ --> checkin.in++;
        output_block1.out++ --> checkin.in++;
        output_block2.out++ --> checkin.in++;
        output_block3.out++ --> checkin.in++;
        output_block4.out++ --> checkin.in++;
        output_block5.out++ --> checkin.in++;
        output_block6.out++ --> checkin.in++;
        output_block7.out++ --> checkin.in++;
        checkin.out --> out.in++;
        input_block0.out --> output_block0.in++;
        input_block1.out --> output_block1.in++;
        input_block2.out --> output_block2.in++;
        input_block3.out --> output_block3.in++;
        input_block4.out --> output_block4.in++;
        input_block5.out --> output_block5.in++;
        input_block6.out --> output_block6.in++;
        input_block7.out --> output_block7.in++;
        //classifier.rest --> toOtherTerminal.in++;
}
