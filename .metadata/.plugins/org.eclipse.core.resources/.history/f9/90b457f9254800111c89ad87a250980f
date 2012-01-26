//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

import org.omnetpp.queueing.WRClassifier_in;
import org.omnetpp.queueing.WRPassiveQueue;
import org.omnetpp.queueing.WRServer;
import org.omnetpp.queueing.Sink;

import org.omnetpp.queueing.WRRouter;
import org.omnetpp.queueing.WRSource;
import org.omnetpp.queueing.WRClassifier_out;
import org.omnetpp.queueing.WRTrigger;


//
// Second try to simulate the current scheduling within the White Rabbit Switch.
// 
//
network WhiteRabbitSwitch7
{
    parameters:
        @display("i=block/users;bgb=1058,668");
    submodules:
        out0: Sink {
            @display("p=953,94;i=block/departure");
        }
        wrQueue0: WRPassiveQueue {
            @display("p=584,94");
        }
        wrQueue1: WRPassiveQueue {
            @display("p=584,152");
        }
        wrQueue2: WRPassiveQueue {
            @display("p=584,209");
        }
        wrQueue3: WRPassiveQueue {
            @display("p=584,267");
        }
        wrQueue4: WRPassiveQueue {
            @display("p=584,357");
        }
        wrQueue5: WRPassiveQueue {
            @display("p=584,415");
        }
        wrQueue6: WRPassiveQueue {
            @display("p=584,472");
        }
        wrQueue7: WRPassiveQueue {
            @display("p=584,530");
        }
        wrServer: WRServer {
            @display("p=765,318;b=40,40");
        }
        wrRouter: WRRouter {
            @display("p=861,318;b=28,29");
        }
        source0: WRSource {
            @display("p=149,94;b=35,37");
        }
        classifier: WRClassifier_in {
            @display("p=346,308;b=31,33");
        }
        source1: WRSource {
            @display("p=149,152;b=35,37");
        }
        source2: WRSource {
            @display("p=149,209;b=35,37");
        }
        source3: WRSource {
            @display("p=149,267;b=35,37");
        }
        source4: WRSource {
            @display("p=149,357;b=35,37");
        }
        source5: WRSource {
            @display("p=149,415;b=35,37");
        }
        source6: WRSource {
            @display("p=149,472;b=35,37");
        }
        source7: WRSource {
            @display("p=149,530;b=35,37");
        }
        out1: Sink {
            @display("p=953,152;i=block/departure");
        }
        out2: Sink {
            @display("p=953,209;i=block/departure");
        }
        out3: Sink {
            @display("p=953,267;i=block/departure");
        }
        out4: Sink {
            @display("p=953,357;i=block/departure");
        }
        out5: Sink {
            @display("p=953,415;i=block/departure");
        }
        out6: Sink {
            @display("p=953,472;i=block/departure");
        }
        out7: Sink {
            @display("p=953,530;i=block/departure");
        }
    connections:
        wrQueue0.out++ --> wrServer.in++;
        wrQueue1.out++ --> wrServer.in++;
        wrQueue2.out++ --> wrServer.in++;
        wrQueue3.out++ --> wrServer.in++;
        wrQueue4.out++ --> wrServer.in++;
        wrQueue5.out++ --> wrServer.in++;
        wrQueue6.out++ --> wrServer.in++;
        wrQueue7.out++ --> wrServer.in++;
        source0.out --> classifier.in++;
        wrServer.out --> wrRouter.in++;
        wrRouter.out++ --> out0.in++;
        classifier.out++ --> wrQueue0.in++;
        classifier.out++ --> wrQueue1.in++;
        classifier.out++ --> wrQueue2.in++;
        classifier.out++ --> wrQueue3.in++;
        classifier.out++ --> wrQueue4.in++;
        classifier.out++ --> wrQueue5.in++;
        classifier.out++ --> wrQueue6.in++;
        classifier.out++ --> wrQueue7.in++;
        source1.out --> classifier.in++;
        source2.out --> classifier.in++;
        source3.out --> classifier.in++;
        source4.out --> classifier.in++;
        source5.out --> classifier.in++;
        source6.out --> classifier.in++;
        source7.out --> classifier.in++;
        wrRouter.out++ --> out1.in++;
        wrRouter.out++ --> out2.in++;
        wrRouter.out++ --> out3.in++;
        wrRouter.out++ --> out4.in++;
        wrRouter.out++ --> out5.in++;
        wrRouter.out++ --> out6.in++;
        wrRouter.out++ --> out7.in++;
}
