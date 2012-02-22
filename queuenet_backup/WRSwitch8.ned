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
import org.omnetpp.queueing.WRSi;
import org.omnetpp.queueing.WRSource8;
import org.omnetpp.queueing.WRClassifier_out;
import org.omnetpp.queueing.WRTrigger;


//
// Second try to simulate the current scheduling within the White Rabbit Switch.
// 
//
network WhiteRabbitSwitch8
{
    parameters:
        @display("i=block/users;bgb=1245,862");
    submodules:
        out0: Sink {
            @display("p=1148,63;i=block/departure");
        }
        source0: WRSource8 {
            @display("p=60,63;b=35,37");
        }
        source1: WRSource8 {
            @display("p=60,160;b=35,37");
        }
        source2: WRSource8 {
            @display("p=60,255;b=35,37");
        }
        source3: WRSource8 {
            @display("p=60,357;b=35,37");
        }
        source4: WRSource8 {
            @display("p=60,454;b=35,37");
        }
        source5: WRSource8 {
            @display("p=60,557;b=35,37");
        }
        source6: WRSource8 {
            @display("p=60,658;b=35,37");
        }
        source7: WRSource8 {
            @display("p=60,759;b=35,37");
        }
        out1: Sink {
            @display("p=1148,160;i=block/departure");
        }
        out2: Sink {
            @display("p=1148,255;i=block/departure");
        }
        out3: Sink {
            @display("p=1148,357;i=block/departure");
        }
        out4: Sink {
            @display("p=1148,454;i=block/departure");
        }
        out5: Sink {
            @display("p=1148,557;i=block/departure");
        }
        out6: Sink {
            @display("p=1148,658;i=block/departure");
        }
        out7: Sink {
            @display("p=1148,759;i=block/departure");
        }
        wrSi: WRSi {
            @display("p=606,411;b=269,481");
        }
    connections:
        source0.out --> wrSi.in++;
        source1.out --> wrSi.in++;
        source2.out --> wrSi.in++;
        source3.out --> wrSi.in++;
        source4.out --> wrSi.in++;
        source5.out --> wrSi.in++;
        source6.out --> wrSi.in++;
        source7.out --> wrSi.in++;
        wrSi.out++ --> out0.in++;
        wrSi.out++ --> out1.in++;
        wrSi.out++ --> out2.in++;
        wrSi.out++ --> out3.in++;
        wrSi.out++ --> out4.in++;
        wrSi.out++ --> out5.in++;
        wrSi.out++ --> out6.in++;
        wrSi.out++ --> out7.in++;
}
