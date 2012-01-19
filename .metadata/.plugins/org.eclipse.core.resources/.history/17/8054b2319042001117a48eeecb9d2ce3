//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

import org.omnetpp.queueing.Classifier;
import org.omnetpp.queueing.WRClassifier_in;
import org.omnetpp.queueing.Delay;
import org.omnetpp.queueing.PassiveQueue;
import org.omnetpp.queueing.WRServer;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;

import org.omnetpp.queueing.WRRouter;
import org.omnetpp.queueing.WRSource;
import org.omnetpp.queueing.WRQueue;
import org.omnetpp.queueing.WRClassifier_out;

//
// Second try to simulate the current scheduling within the White Rabbit Switch.
// 
//
network WhiteRabbitSwitch5
{
    parameters:
        @display("i=block/users;bgb=766,515");
    submodules:
        out0: Sink {
            @display("p=708,34;i=block/departure");
        }
        wrQueue0: WRQueue {
            @display("p=362,35");
        }
        out1: Sink {
            @display("p=708,101;i=block/departure");
        }
        out2: Sink {
            @display("p=708,162;i=block/departure");
        }
        out3: Sink {
            @display("p=708,229;i=block/departure");
        }
        out4: Sink {
            @display("p=708,286;i=block/departure");
        }
        out5: Sink {
            @display("p=708,353;i=block/departure");
        }
        out6: Sink {
            @display("p=708,414;i=block/departure");
        }
        out7: Sink {
            @display("p=708,482;i=block/departure");
        }
        classifier_out: WRClassifier_out {
            @display("p=528,259;b=27,28");
        }
        wrQueue1: WRQueue {
            @display("p=362,93");
        }
        wrQueue2: WRQueue {
            @display("p=362,150");
        }
        wrQueue3: WRQueue {
            @display("p=362,208");
        }
        wrQueue4: WRQueue {
            @display("p=362,298");
        }
        wrQueue5: WRQueue {
            @display("p=362,356");
        }
        wrQueue6: WRQueue {
            @display("p=362,413");
        }
        wrQueue7: WRQueue {
            @display("p=362,471");
        }
        source: WRSource {
            @display("p=37,35;b=28,22");
        }
        source1: WRSource {
            @display("p=37,93;b=28,22");
        }
        source2: WRSource {
            @display("p=37,150;b=28,22");
        }
        source3: WRSource {
            @display("p=37,208;b=28,22");
        }
        source4: WRSource {
            @display("p=37,298;b=28,22");
        }
        source5: WRSource {
            @display("p=37,356;b=28,22");
        }
        source6: WRSource {
            @display("p=37,413;b=28,22");
        }
        source7: WRSource {
            @display("p=37,471;b=28,22");
        }
    connections:
        classifier_out.out++ --> out0.in++;
        classifier_out.out++ --> out1.in++;
        classifier_out.out++ --> out2.in++;
        classifier_out.out++ --> out3.in++;
        classifier_out.out++ --> out4.in++;
        classifier_out.out++ --> out5.in++;
        classifier_out.out++ --> out6.in++;
        classifier_out.out++ --> out7.in++;
        wrQueue0.out --> classifier_out.in++;
        wrQueue1.out --> classifier_out.in++;
        wrQueue2.out --> classifier_out.in++;
        wrQueue3.out --> classifier_out.in++;
        wrQueue4.out --> classifier_out.in++;
        wrQueue5.out --> classifier_out.in++;
        wrQueue6.out --> classifier_out.in++;
        wrQueue7.out --> classifier_out.in++;
        source.out --> wrQueue0.in++;
        source1.out --> wrQueue1.in++;
        source2.out --> wrQueue2.in++;
        source3.out --> wrQueue3.in++;
        source4.out --> wrQueue4.in++;
        source5.out --> wrQueue5.in++;
        source6.out --> wrQueue6.in++;
        source7.out --> wrQueue7.in++;
}
